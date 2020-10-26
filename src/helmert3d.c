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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERS 0.51

int main(int argc, char* argv[]){
 FILE *parmfile;
 FILE *ifile;
 FILE *ofile;
 char ifilename[256];
 char buf[256];
 double tx=0.0;
 double ty=0.0;
 double tz=0.0;
 double m=0.0;
 double r11=0.0;
 double r12=0.0;
 double r13=0.0;
 double r21=0.0;
 double r22=0.0;
 double r23=0.0;
 double r31=0.0;
 double r32=0.0;
 double r33=0.0;
 double x=0.0;
 double y=0.0;
 double z=0.0;
 double xout=0.0;
 double yout=0.0;
 double zout=0.0;

   printf("\n***********************************\n");
   printf(  "* 3D-Helmert Transformation v%1.2f *\n",VERS);
   printf(  "*           26.10.2020            *\n");
   printf(  "*       (c) U. Niethammer         *\n");
   printf(  "***********************************\n");
   if(argc < 3){
      printf("Syntax:  %s [xyz_filename] [helmert_param_filename]\n\n",argv[0]);
      printf("helmert parameter file format:\n");
      printf(" r11 r12 r13\n r21 r22 r23\n r31 r32 r33\n tx ty tz\n m\n\n");
      printf("xyz data file format:\n");
      printf(" x[1] y[1] z[1]\n ..   ..   ..\n ..   ..   ..\n x[n] y[n] z[n]\n\n");
      exit(1);
   }
   memset(ifilename,0,sizeof(ifilename));
   strcpy(ifilename,argv[1]);
   ifile = fopen( ifilename, "r");
   if(ifile == NULL){
      printf("Error opening %s\r\n",ifilename);
      exit(-1);
   }
   memset(ifilename,0,sizeof(ifilename));
   strcpy(ifilename,argv[2]);
   parmfile = fopen( ifilename, "r");
   if(parmfile == NULL){
      printf("Error opening %s\r\n",ifilename);
      exit(-1);
   }
   ofile = fopen( "transformed.xyz", "w");
   if(ofile == NULL){
      printf("Error writing %s\r\n","ixyz_helmert.xyz");
      exit(-1);
   }

   printf("reading helmert parameters...\n");
   if(fgets( buf, 128, parmfile)!=NULL){
      sscanf( buf, "%lf %lf %lf", &r11, &r12, &r13);
   }
   else{
      printf("Error reading %s\n",ifilename);
   }
   if(fgets( buf, 128, parmfile)!=NULL){
      sscanf( buf, "%lf %lf %lf", &r21, &r22, &r23);
   }
   else{
      printf("Error reading %s\n",ifilename);
   }
   if(fgets( buf, 128, parmfile)!=NULL){
      sscanf( buf, "%lf %lf %lf", &r31, &r32, &r33);
   }
   else{
      printf("Error reading %s\n",ifilename);
   }
   if(fgets( buf, 128, parmfile)!=NULL){
      sscanf( buf, "%lf %lf %lf", &tx, &ty, &tz);
   }
   else{
      printf("Error reading %s\n",ifilename);
   }
   if(fgets( buf, 128, parmfile)!=NULL){
      sscanf( buf, "%lf", &m);
   }
   else{
      printf("Error reading %s\n",ifilename);
   }

   //if(m>=0.0)m+=1.0;//ppm to real scaling factor
   //if(m<0.0)m-=1.0; //ppm to real scaling factor
   printf("%lf %lf %lf\n", r11 , r12 , r13);
   printf("%lf %lf %lf\n", r21 , r22 , r23);
   printf("%lf %lf %lf\n", r31 , r32 , r33);
   printf("%lf %lf %lf\n", tx , ty , tz);
   printf("%lf\n", m);

   printf("...done\n");


   printf("starting transformation...\n");
   while(fgets( buf, 128, ifile)!=NULL){
      sscanf( buf, "%lf %lf %lf", &x, &y, &z);
      xout=tx+m*(x*r11+y*r12+z*r13);
      yout=ty+m*(x*r21+y*r22+z*r23);
      zout=tz+m*(x*r31+y*r32+z*r33);
      fprintf(ofile,"%lf %lf %lf\n", xout , yout , zout);
   }
   printf("...done\nResults written to transformed.xyz\n");
   return(0);
}


