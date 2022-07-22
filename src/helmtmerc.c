/* Copyright (C) 2021 U. Niethammer,  https://github.com/dr-ni/helmert3d
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "helmert3d.h"

static size_t get_m_size(char *filename)
{
    FILE *ptsfile;
    char buf[256];
    size_t linecount=0;

    memset(buf, 0, sizeof(buf));
    ptsfile = fopen( filename, "r");
    if(ptsfile == NULL)
    {
        fprintf(stderr,"Error opening %s\r\n",filename);
        exit(EXIT_FAILURE);
    }
    // Count points
    while(fgets( buf, 128, ptsfile)!=NULL)
    {
        if( strcmp(buf,"\n") != 0 && strcmp(buf,"\r\n") != 0 && strcmp(buf,"\0") != 0)
        {
            linecount++;
        }
        else
        {
            fprintf(stderr,"Error, %s: wrong data format\n",filename);
            exit(EXIT_FAILURE);
        }
    }
    (void)fclose(ptsfile);
    return(linecount);
}

int main(int argc, char* argv[])
{
    FILE *ifile;
    FILE *cfile;
    FILE *zfile;
    FILE *ofile;
    char *ifilename = "";
    char *cfilename = "";
    char *zfilename = "";
    char *ofilename = "out.txt";
    char *command = "xyz";
    char ibuf[256], cbuf[256], zbuf[256], name[128];
    double a = DEFAULT_A, b = DEFAULT_B, flattening, ro, roi, zone_size, zone_number, zscale, lo, xo, yo;
    double e2, n, rectifyingRadius, A, B, C, D, beta1, beta2, beta3, beta4;
    double bt, lt, ht, xt, yt, phi, lambda, phis, phic, phiStar, xiPrim, etaPrim, xi, eta;


    size_t l = 0;
    int stat = 0;

    fprintf(stdout, "\n*******************************\n");
    fprintf(stdout,   "*      helmtmerc v%s       *\n",VERS);
    fprintf(stdout,   "*   (c) %s %s    *\n",C_AUTHOR,C_YEAR);
    fprintf(stdout,   "*******************************\n");

    if(argc < 5)
    {
        fprintf(stdout,"\nSyntax:  %s commands blh|xyh_src_infilename ellipsoid_infilename zone_infilename [blh|xyh_dest_infilename]\n\n",argv[0]);
        fprintf(stdout,"commands:\n");
        fprintf(stdout," xyh - convert B-L-H to x-y-H\n");
        fprintf(stdout," blh - convert x-y-H to B-L-H\n\n");
        fprintf(stdout,"ellipsoid file format:\n");
        fprintf(stdout," Name a b\n\n");
        fprintf(stdout,"zone file format:\n");
        fprintf(stdout," zone_size zone_number [zone_scale] [L0] [x0] [y0]\n\n");
        fprintf(stdout,"blh data file format:\n");
        fprintf(stdout," B[1] L[1] H[1]\n ..   ..   ..\n ..   ..   ..\n B[n] L[n] H[n]\n\n");
        fprintf(stdout,"xyh data file format:\n");
        fprintf(stdout," x[1] y[1] H[1]\n ..   ..   ..\n ..   ..   ..\n x[n] y[n] H[n]\n\n");
        exit(EXIT_FAILURE);
    }
    command = argv[1];
    if (!strcmp(command,"xyh"))
    {
        fprintf(stdout,"B-L-H -> x-y-H...\n");
    }
    else if (!strcmp(command,"blh"))
    {
        fprintf(stdout,"x-y-H -> B-L-H...\n");
    }
    else
    {
        fprintf(stderr,"Unknow command %s\n",command);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Reading points...\n");
    ifilename = argv[2];
    l = get_m_size(ifilename);
    fprintf(stdout,"Found %lu points\n",(unsigned long)l);
    ifile = fopen( ifilename, "r");
    if(ifile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",ifilename);
        exit(EXIT_FAILURE);
    }
    cfilename = argv[3];
    l = get_m_size(cfilename);
    fprintf(stdout,"Found ellipsoid\n");
    cfile = fopen( cfilename, "r");
    if(cfile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",cfilename);
        exit(EXIT_FAILURE);
    }
    zfilename = argv[4];
    l = get_m_size(cfilename);
    fprintf(stdout,"Found tmerc\n");
    zfile = fopen( zfilename, "r");
    if(zfile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",zfilename);
        exit(EXIT_FAILURE);
    }
    if(argc > 5)
    {
        ofilename = argv[5];
        ofile = fopen( ofilename, "w");
    }
    else
        ofile = stdout;

    if(ofile == NULL)
    {
        fprintf(stderr,"Error writing %s\n",ofilename);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout,"Starting calculation...\n");
    fgets( cbuf, 256, cfile);
    stat = sscanf( cbuf, "%s %lf %lf", name, &a, &b);
    if(stat != 3)
    {
        fprintf(stderr,"Error wrong data format in %s\n",cfilename);
        exit(EXIT_FAILURE);
    }
    flattening = (a - b) / a;
    fprintf(stdout,"Ellipsoid %s, a = %lf, b = %lf, f = %lf\n", name, a, b, flattening);
    fgets( zbuf, 256, zfile);
    stat = sscanf( zbuf, "%lf %lf %lf %lf %lf %lf", &zone_size, &zone_number, &zscale, &lo, &xo, &yo);
    if(stat < 2)
    {
        fprintf(stderr,"Error wrong data format in %s\n",zfilename);
        exit(EXIT_FAILURE);
    }
    if(stat < 3)
        zscale = 1.0;
    if(stat < 4)
        lo = (zone_number - 0.5) * zone_size;
    if(stat < 5)
        xo = 0;
    if(stat < 6)
        yo = (zone_number + 0.5) * 1E6;
    fprintf(stdout,"Tmerc zs = %g, zn = %g, scale = %lf, L0 = %lf, x0 = %lf, y0 = %lf\n", zone_size, zone_number, zscale, lo, xo, yo);
    ro = DEG2RAD;
    roi = RAD2DEG;
    e2 = flattening * (2.0 - flattening);
    n = flattening / (2.0 - flattening);
    rectifyingRadius = a / (1.0 + n) * (1.0 + 0.25 * n * n + 0.015625 * n * n * n * n);
    if (!strcmp(command,"xyh"))
    {
        A = e2;
        B = (5.0 * e2 * e2 - e2 * e2 * e2) / 6.0;
        C = (104.0 * e2 * e2 * e2 - 45.0 * e2 * e2 * e2 * e2) / 120.0;
        D = (1237.0 * e2 * e2 * e2 * e2) / 1260.0;
        beta1 = 1.0/2.0 * n - 2.0/3.0 * n * n + 5.0/16.0 * n * n * n + 41.0/180.0 * n * n * n * n;
        beta2 = 13.0/48.0 * n * n - 3.0/5.0 * n * n * n + 557.0/1440.0 * n * n * n * n;
        beta3 = 61.0/240.0 * n * n * n - 103.0/140.0 * n * n * n * n;
        beta4 = 49561.0/161280.0 * n * n * n * n;

        while(fgets( ibuf, 256, ifile)!=NULL)
        {
            stat=sscanf( ibuf, "%lf %lf %lf", &bt, &lt, &ht);
            if(stat != 3)
            {
                fprintf(stderr,"Error wrong data format in %s\n",ifilename);
                exit(EXIT_FAILURE);
            }
            phi = ro * bt;
            lambda = ro * (lt - lo);
            phis = sin(phi);
            phic = cos(phi);
            phiStar = phi - phis * phic *
                    (A + B * phis * phis + C * phis * phis * phis * phis + D * phis * phis * phis * phis * phis * phis);

            xiPrim = atan(tan(phiStar) / cos(lambda));
            etaPrim = atanh(cos(phiStar) * sin(lambda));
            xt = xo + zscale * rectifyingRadius * (xiPrim
               + beta1 * sin(2.0*xiPrim) * cosh(2.0*etaPrim)
               + beta2 * sin(4.0*xiPrim) * cosh(4.0*etaPrim)
               + beta3 * sin(6.0*xiPrim) * cosh(6.0*etaPrim)
               + beta4 * sin(8.0*xiPrim) * cosh(8.0*etaPrim));
            yt = yo + zscale * rectifyingRadius * (etaPrim
               + beta1 * cos(2.0*xiPrim) * sinh(2.0*etaPrim)
               + beta2 * cos(4.0*xiPrim) * sinh(4.0*etaPrim)
               + beta3 * cos(6.0*xiPrim) * sinh(6.0*etaPrim)
               + beta4 * cos(8.0*xiPrim) * sinh(8.0*etaPrim));
            fprintf(ofile,"%lf %lf %lf\n", xt , yt , ht);
        }
    }
    else if (!strcmp(command,"blh"))
    {
        A = e2 + e2 * e2 + e2 * e2 * e2 + e2 * e2 * e2 * e2;
        B = -(7.0 * e2 * e2 + 17.0 * e2 * e2 * e2 + 30.0 * e2 * e2 * e2 * e2) / 6.0;
        C = (224.0 * e2 * e2 * e2 + 889.0 * e2 * e2 * e2 * e2) / 120.0;
        D = -(4279.0 * e2 * e2 * e2 * e2) / 1260.0;
        beta1 = 1.0/2.0 * n - 2.0/3.0 * n * n + 37.0/96.0 * n * n * n - 1.0/360.0 * n * n * n * n;
        beta2 = 1.0/48.0 * n * n + 1.0/15.0 * n * n * n - 437.0/1440.0 * n * n * n * n;
        beta3 = 17.0/480.0 * n * n * n - 37.0/840.0 * n * n * n * n;
        beta4 = 4397.0/161280.0 * n * n * n * n;

        while(fgets( ibuf, 256, ifile)!=NULL)
        {
            stat=sscanf( ibuf, "%lf %lf %lf", &xt, &yt, &ht);
            if(stat != 3)
            {
                fprintf(stderr,"Error wrong data format in %s\n",ifilename);
                exit(EXIT_FAILURE);
            }
            xi = (xt - xo) / (zscale * rectifyingRadius);
            eta = (yt - yo) / (zscale * rectifyingRadius);
            xiPrim = xi
                   - beta1 * sin(2.0*xi) * cosh(2.0*eta)
                   - beta2 * sin(4.0*xi) * cosh(4.0*eta)
                   - beta3 * sin(6.0*xi) * cosh(6.0*eta)
                   - beta4 * sin(8.0*xi) * cosh(8.0*eta);
            etaPrim = eta
                    - beta1 * cos(2.0*xi) * sinh(2.0*eta)
                    - beta2 * cos(4.0*xi) * sinh(4.0*eta)
                    - beta3 * cos(6.0*xi) * sinh(6.0*eta)
                    - beta4 * cos(8.0*xi) * sinh(8.0*eta);
            phiStar = asin(sin(xiPrim) / cosh(etaPrim));
            lambda = atan(sinh(etaPrim) / cos(xiPrim));
            phis = sin(phiStar);
            phic = cos(phiStar);
            phi = phiStar
                    + phis * phic * ( A
                                    + B * phis * phis
                                    + C * phis * phis * phis * phis
                                    + D * phis * phis * phis * phis * phis * phis);
            bt = roi * phi;
            lt = lo + roi * lambda;
            fprintf(ofile,"%3.8lf %3.8lf %lf\n", bt , lt , ht);
        }
    }
    else
    {
        fprintf(stderr,"Unknow command %s\n",command);
        exit(EXIT_FAILURE);
    }
    if(argc > 5)
    {
        fprintf(stdout,"\nResults written to %s\n", ofilename);
        (void)fclose(ofile);
    }
    (void)fclose(ifile);
    (void)fclose(cfile);

    fprintf(stdout,"...done\n");
    return(0);
}
