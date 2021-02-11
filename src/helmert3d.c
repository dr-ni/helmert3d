/* Copyright (C) 2011 U. Niethammer,  http://helmparms3d.sourceforge.net/
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
    FILE *parmfile;
    FILE *ifile;
    FILE *ofile;
    char *ifilename = "";
    char *pfilename = "";
    char *ofilename = "transformed.xyz";
    char buf[256];
    double tx=0.0, ty=0.0, tz=0.0;
    double m=0.0;
    double r11=0.0, r12=0.0, r13=0.0;
    double r21=0.0, r22=0.0, r23=0.0;
    double r31=0.0, r32=0.0, r33=0.0;
    double x=0.0, y=0.0, z=0.0;
    double xout=0.0, yout=0.0, zout=0.0;
    size_t l=0;
    int stat=0;

    fprintf(stdout,"\n*******************************\n");
    fprintf(stdout,  "*      helmert3d v%s       *\n",VERS);
    fprintf(stdout,  "*   (c) U. Niethammer 2020    *\n");
    fprintf(stdout,  "*******************************\n");

    if(argc < 3)
    {
        fprintf(stdout,"\nSyntax:  %s [xyz_src_infilename] [helmert_param_infilename] [xyz_dest_outfilename]\n\n",argv[0]);
        fprintf(stdout,"helmert parameter file format:\n");
        fprintf(stdout," r11 r12 r13\n r21 r22 r23\n r31 r32 r33\n tx ty tz\n s\n\n");
        fprintf(stdout,"xyz data file format:\n");
        fprintf(stdout," x[1] y[1] z[1]\n ..   ..   ..\n ..   ..   ..\n x[n] y[n] z[n]\n\n");
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
    pfilename = argv[2];
    l = get_m_size(pfilename);
    if(l != 5)
    {
        fprintf(stderr,"Error wrong format in %s\n",pfilename);
        exit(EXIT_FAILURE);
    }
    parmfile = fopen( pfilename, "r");
    if(parmfile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",pfilename);
        exit(EXIT_FAILURE);
    }
    if(argc > 3)
    {
        ofilename = argv[3];
    }
    ofile = fopen( ofilename, "w");
    if(ofile == NULL)
    {
        fprintf(stderr,"Error writing %s\n",ofilename);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout,"Reading helmert parameters...\n");
    if(fgets( buf, 128, parmfile)!=NULL)
    {
        stat=sscanf(buf, "%lf %lf %lf", &r11, &r12, &r13);
        if(stat != 3)
        {
            fprintf(stderr,"Error wrong data format in %s\n",pfilename);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr,"Error reading %s\n",pfilename);
        exit(EXIT_FAILURE);
    }
    if(fgets( buf, 128, parmfile)!=NULL)
    {
        stat=sscanf( buf, "%lf %lf %lf", &r21, &r22, &r23);
        if(stat != 3)
        {
            fprintf(stderr,"Error wrong data format in %s\n",pfilename);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr,"Error reading %s\n",pfilename);
        exit(EXIT_FAILURE);
    }
    if(fgets( buf, 128, parmfile)!=NULL)
    {
        stat=sscanf( buf, "%lf %lf %lf", &r31, &r32, &r33);
        if(stat != 3)
        {
            fprintf(stderr,"Error wrong data format in %s\n",pfilename);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr,"Error reading %s\n",pfilename);
        exit(EXIT_FAILURE);
    }
    if(fgets( buf, 128, parmfile)!=NULL)
    {
        stat=sscanf( buf, "%lf %lf %lf", &tx, &ty, &tz);
        if(stat != 3)
        {
            fprintf(stderr,"Error wrong data format in %s\n",pfilename);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr,"Error reading %s\n",pfilename);
        exit(EXIT_FAILURE);
    }
    if(fgets( buf, 128, parmfile)!=NULL)
    {
        stat=sscanf( buf, "%lf", &m);
        if(stat != 1)
        {
            fprintf(stderr,"Error wrong data format in %s\n",pfilename);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr,"Error reading %s\n",pfilename);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout,"%lf %lf %lf\n", r11 , r12 , r13);
    fprintf(stdout,"%lf %lf %lf\n", r21 , r22 , r23);
    fprintf(stdout,"%lf %lf %lf\n", r31 , r32 , r33);
    fprintf(stdout,"%lf %lf %lf\n", tx , ty , tz);
    fprintf(stdout,"%lf\n", m);

    fprintf(stdout,"...done\n");

    fprintf(stdout,"Starting transformation...\n");
    while(fgets( buf, 128, ifile)!=NULL)
    {
        stat=sscanf( buf, "%lf %lf %lf", &x, &y, &z);
        if(stat != 3)
        {
            fprintf(stderr,"Error wrong data format in %s\n",ifilename);
            exit(EXIT_FAILURE);
        }
        xout=tx+m*(x*r11+y*r12+z*r13);
        yout=ty+m*(x*r21+y*r22+z*r23);
        zout=tz+m*(x*r31+y*r32+z*r33);
        fprintf(ofile,"%lf %lf %lf\n", xout , yout , zout);
    }
    fprintf(stdout,"...done\nResults written to %s\n", ofilename);
    (void)fclose(ifile);
    (void)fclose(parmfile);
    (void)fclose(ofile);
    return(0);
}
