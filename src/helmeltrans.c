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
    FILE *ofile;
    char *ifilename = "";
    char *cfilename = "";
    char *ofilename = "out.txt";
    char *command = "xyz";
    char ibuf[256], cbuf[256], name[128];
    double a = DEFAULT_A, b = DEFAULT_B, flattening, ro, roi;
    double a2, b2, bc, bs, lc, ls, ta, tb, nb, lt, bt, ht, xt, yt, zt;
    double x2, y2, z2, r2, r, e2, et2, kf, kg, kc, ks, ks2, kp, kq, r0, re, re2, ku, kv, kz, z0;


    size_t l = 0;
    int stat = 0;

    fprintf(stdout, "\n*******************************\n");
    fprintf(stdout,   "*     helmeltrans v%s      *\n",VERS);
    fprintf(stdout,   "*   (c) %s %s    *\n",C_AUTHOR,C_YEAR);
    fprintf(stdout,   "*******************************\n");

    if(argc < 4)
    {
        fprintf(stdout,"\nSyntax:  %s commands blh|xyz_src_infilename ellipsoid_src_infilename [blh|xyz_dest_infilename]\n\n",argv[0]);
        fprintf(stdout,"commands:\n");
        fprintf(stdout," xyz - convert B-L-H to X-Y-Z\n");
        fprintf(stdout," blh - convert X-Y-Z to B-L-H\n\n");
        fprintf(stdout,"ellipsoid file format:\n");
        fprintf(stdout," Name a b\n\n");
        fprintf(stdout,"blh data file format:\n");
        fprintf(stdout," B[1] L[1] H[1]\n ..   ..   ..\n ..   ..   ..\n B[n] L[n] H[n]\n\n");
        fprintf(stdout,"xyz data file format:\n");
        fprintf(stdout," X[1] Y[1] Z[1]\n ..   ..   ..\n ..   ..   ..\n X[n] Y[n] Z[n]\n\n");
        exit(EXIT_FAILURE);
    }
    command = argv[1];
    if (!strcmp(command,"xyz"))
    {
        fprintf(stdout,"B-L-H -> X-Y-Z...\n");
    }
    else if (!strcmp(command,"blh"))
    {
        fprintf(stdout,"X-Y-Z -> B-L-H...\n");
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
    if(argc > 4)
    {
        ofilename = argv[4];
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
    a2 = a * a;
    b2 = b * b;
    ro = DEG2RAD;
    roi = RAD2DEG;
    if (!strcmp(command,"xyz"))
    {
        while(fgets( ibuf, 256, ifile)!=NULL)
        {
            stat=sscanf( ibuf, "%lf %lf %lf", &bt, &lt, &ht);
            if(stat != 3)
            {
                fprintf(stderr,"Error wrong data format in %s\n",ifilename);
                exit(EXIT_FAILURE);
            }
            bc = cos(ro * bt);
            bs = sin(ro * bt);
            lc = cos(ro * lt);
            ls = sin(ro * lt);
            ta = a * bc;
            tb = b * bs;
            nb = a2 / sqrt(ta * ta + tb * tb);
            xt = (nb + ht) * bc * lc;
            yt = (nb + ht) * bc * ls;
            zt = (b2 / a2 * nb + ht) * bs;
            fprintf(ofile,"%lf %lf %lf\n", xt , yt , zt);
        }
    }
    else if (!strcmp(command,"blh"))
    {
        while(fgets( ibuf, 256, ifile)!=NULL)
        {
            stat=sscanf( ibuf, "%lf %lf %lf", &xt, &yt, &zt);
            if(stat != 3)
            {
                fprintf(stderr,"Error wrong data format in %s\n",ifilename);
                exit(EXIT_FAILURE);
            }
            x2 = xt * xt;
            y2 = yt * yt;
            r2 = x2 + y2;
            r = sqrt(r2);
            e2 = 1.0 - b2 / a2;
            et2 = (a2 - b2) / b2;
            z2 = zt * zt;
            kf = 54.0 * b2 * z2;
            kg = r2 + (1 - e2) * z2 - e2 * (a2 - b2);
            kc = e2 * e2 * kf * r2 / (kg * kg * kg);
            ks = cbrt(1.0 + kc + sqrt(kc * kc + 2.0 * kc));
            ks2 = ks + 1.0 / ks + 1.0;
            kp = kf / (3.0 * ks2 * ks2 * kg * kg);
            kq = sqrt(1.0 + 2.0 * e2 * e2 * kp);
            r0 = -kp * e2 * r / (1.0 + kq) + sqrt(0.5 * a2 *(1.0 + 1.0 / kq) - kp * (1.0 - e2) * z2 / (kq * (1.0 + kq)) - 0.5 * kp * r2);
            re = r - e2 * r0;
            re2 = re * re;
            ku = sqrt(re2 + z2);
            kv = sqrt(re2 + (1.0 - e2) * z2);
            kz = b2 / (a * kv);
            z0 = kz * zt;
            ht = ku * (1.0 - kz);
            bt = roi * atan((zt + et2 * z0) / r);
            lt = roi * atan2(yt, xt);
            fprintf(ofile,"%3.8lf %3.8lf %lf\n", bt , lt , ht);
        }
    }
    else
    {
        fprintf(stderr,"Unknow command %s\n",command);
        exit(EXIT_FAILURE);
    }
    if(argc > 4)
    {
        fprintf(stdout,"\nResults written to %s\n", ofilename);
        (void)fclose(ofile);
    }
    (void)fclose(ifile);
    (void)fclose(cfile);

    fprintf(stdout,"...done\n");
    return(0);
}
