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
    FILE *cfile;
    FILE *ofile;
    char *ifilename = "";
    char *cfilename = "";
    char *ofilename = "diff.xyz";
    char ibuf[256], cbuf[256];
    double xs=0.0, ys=0.0, zs=0.0;
    double xc=0.0, yc=0.0, zc=0.0;
    double xout=0.0, yout=0.0, zout=0.0;
    size_t l=0;
    int stat=0;

    fprintf(stdout,"\n*******************************\n");
    fprintf(stdout,  "*      helmdiff3d v%s      *\n",VERS);
    fprintf(stdout,  "*   (c) %s %s    *\n",C_AUTHOR,C_YEAR);
    fprintf(stdout,  "*******************************\n");

    if(argc < 3)
    {
        fprintf(stdout,"\nSyntax:  %s xyz_src_infilename xyz_dest_infilename [xyz_diff_outfilename]\n\n",argv[0]);
        fprintf(stdout,"xyz data file format:\n");
        fprintf(stdout," X[1] Y[1] Z[1]\n ..   ..   ..\n ..   ..   ..\n X[n] Y[n] Z[n]\n\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Reading points...\n");
    ifilename = argv[1];
    l = get_m_size(ifilename);
    fprintf(stdout,"Found %lu points\n",(unsigned long)l);
    ifile = fopen( ifilename, "r");
    if(ifile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",ifilename);
        exit(EXIT_FAILURE);
    }
    cfilename = argv[2];
    l = get_m_size(cfilename);
    fprintf(stdout,"Found %lu points\n",(unsigned long)l);
    cfile = fopen( cfilename, "r");
    if(cfile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",cfilename);
        exit(EXIT_FAILURE);
    }
    if(argc > 3)
    {
        ofilename = argv[3];
        ofile = fopen( ofilename, "w");
    }
    else
        ofile = stdout;
    if(ofile == NULL)
    {
        fprintf(stderr,"Error writing %s\n",ofilename);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout,"Starting diff...\n");
    while(fgets( ibuf, 128, ifile)!=NULL && fgets( cbuf, 128, cfile)!=NULL)
    {
        stat=sscanf( ibuf, "%lf %lf %lf", &xs, &ys, &zs);
        if(stat != 3)
        {
            fprintf(stderr,"Error wrong data format in %s\n",ifilename);
            exit(EXIT_FAILURE);
        }
        stat=sscanf( cbuf, "%lf %lf %lf", &xc, &yc, &zc);
        if(stat != 3)
        {
            fprintf(stderr,"Error wrong data format in %s\n",cfilename);
            exit(EXIT_FAILURE);
        }
        xout=xs-xc;
        yout=ys-yc;
        zout=zs-zc;
        fprintf(ofile,"%lf %lf %lf\n", xout , yout , zout);
    }
    if(argc > 3)
    {
        fprintf(stdout,"\nResults written to %s\n", ofilename);
        (void)fclose(ofile);
    }
    (void)fclose(ifile);
    (void)fclose(cfile);

    fprintf(stdout,"...done\n");
    return(0);
}
