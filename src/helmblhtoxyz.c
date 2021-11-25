/* Copyright (C) 2020 U. Niethammer,  https://github.com/dr-ni/helmert3d
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
    FILE *ofile;
    char *ifilename = "";
    char *ofilename = "out.xyz";
    char ibuf[256], name[128];
    double a = 6378137.0, b = 6356752.3142;
    double bc, bs, lc, ls, ta, tb, nb, lt, bt, ht, xt, yt, zt;
    size_t s = 0, i = 0;
    int stat = 0;

    fprintf(stdout,"\n*******************************\n");
    fprintf(stdout,  "*      helmblhtoxyz v%s      *\n",VERS);
    fprintf(stdout,  "*   (c) U. Niethammer 2020    *\n");
    fprintf(stdout,  "*******************************\n");

    if(argc < 2)
    {
        fprintf(stdout,"\nSyntax:  %s blh_src_infilename [xyz_dest_infilename]\n\n",argv[0]);
        fprintf(stdout,"blh data file format:\n");
        fprintf(stdout," Name a b\n B[1] L[1] H[1]\n ..   ..   ..\n ..   ..   ..\n B[n] L[n] H[n]\n\n");
        fprintf(stdout,"xyz data file format:\n");
        fprintf(stdout," X[1] Y[1] Z[1]\n ..   ..   ..\n ..   ..   ..\n X[n] Y[n] Z[n]\n\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Reading points...\n");
    ifilename = argv[1];
    s = get_m_size(ifilename) - 1;
    fprintf(stdout,"Found %lu points\n",(unsigned long)s);
    ifile = fopen( ifilename, "r");
    if(ifile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",ifilename);
        exit(EXIT_FAILURE);
    }
    if(argc > 2)
    {
        ofilename = argv[2];
    }
    ofile = fopen( ofilename, "w");
    if(ofile == NULL)
    {
        fprintf(stderr,"Error writing %s\n",ofilename);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout,"Starting calculate...\n");
    while(fgets( ibuf, 128, ifile)!=NULL)
    {
        if (i == 0)
        {
            stat = sscanf( ibuf, "%s %lf %lf", &name, &a, &b);
            if(stat != 3)
            {
                fprintf(stderr,"Error wrong data format in %s\n",ifilename);
                exit(EXIT_FAILURE);
            }
            fprintf(stdout,"Ellipsoid %s, a = %lf, b = %lf\n", name, a, b);
        }
        else
        {
            stat=sscanf( ibuf, "%lf %lf %lf", &bt, &lt, &ht);
            if(stat != 3)
            {
                fprintf(stderr,"Error wrong data format in %s\n",ifilename);
                exit(EXIT_FAILURE);
            }
            bc = cos(M_PI / 180.0 * bt);
            bs = sin(M_PI / 180.0 * bt);
            lc = cos(M_PI / 180.0 * lt);
            ls = sin(M_PI / 180.0 * lt);
            ta = a * bc;
            tb = b * bs;
            nb = (a * a) / sqrt(ta * ta + tb * tb);
            xt = (nb + ht) * bc * lc;
            yt = (nb + ht) * bc * ls;
            zt = (b * b / a / a * nb + ht) * bs;
            fprintf(ofile,"%lf %lf %lf\n", xt , yt , zt);
        }
        i++;
    }
    fprintf(stdout,"...done\nResults written to %s\n", ofilename);
    (void)fclose(ifile);
    (void)fclose(ofile);
    return(0);
}
