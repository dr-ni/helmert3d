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
#include "svdm.h"
#define VERS 0.51
#define DEBUG 0

void freematrix(int m, double ** u)
{
    int i=0;
    for(i = 0; i < m; i++)
    {
        free(u[i]);
    }
    free(u);
}

void freevector(double * u)
{
    free(u);
}

double ** matrix(int m, int n, double ** u)
{
    int i=0;
    u = malloc(m * sizeof(*u));
    for(i = 0; i < m; i++)
    {
        u[i] = calloc(n * sizeof(*u[0]),sizeof(double *));
    }
    return u;
}

double * vector(int m, double * u)
{
    u = malloc(m * sizeof(*u));
    return u;
}

//Plot Vector
void plot_vector(FILE *target, int dim,double *vector)
{
    int count;
    for(count=0; count<dim; count++)
    {
        fprintf(target, "%10.10lf ",vector[count]);
    }
    fprintf(target, "\n");
}

//Plot Matrix
void plot_matrix(FILE *target, int b,int h,double **matrix_a)
{
    int i=0;
    int j=0;
    for(i=0; i<b; i++)
    {
        for(j=0; j<h; j++)
        {
            fprintf(target, "%10.10lf ", matrix_a[i][j]);
        }
        fprintf(target, "\n");
    }
}

//Matrix mit Vektor multiplizieren
void matrix_multiply(int h,int b,double ** matrix,double *invector,double *outvector)
{
    int count;
    int count1;
    for(count=0; count<h; count++)outvector[count]=0.0;
    for(count=0; count<h; count++)
    {
        for(count1=0; count1<b; count1++)
        {
            outvector[count]+=matrix[count][count1]*invector[count1];
        }
    }
}

void matmult(double **mat1, int m1, int n1, double **mat2, int m2, int n2, double **mat3, int m3, int n3)
{
    int i=0, j=0, k=0;
    double sum=0.0;
    if (m1 != n2)
    {
        fprintf(stderr,"Matrix m1 cannot be multiplied to m2.\n");
        exit(1);
    }
    for(i=0; i<m1; i++)
    {
        for(j=0; j<n1; j++)
        {
            sum=0.0;
            for(k=0; k<n1; k++)
            {
                sum+=mat1[i][k]*mat2[k][j];
            }
            mat3[i][j]=sum;
        }
    }
}

void transpose_matrix(int n, int m, double **src_matrix, double **dest_matrix)
{
    int i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            dest_matrix[i][j]=src_matrix[j][i];
        }
    }
}

void init_matrix(int n, int m, double **matrix)
{
    int i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            matrix[i][j]=0.0;
        }
    }
}

void copy_matrix(int n, int m, double **src_matrix, double **dest_matrix)
{
    int i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            dest_matrix[i][j]=src_matrix[i][j];
        }
    }
}

void scal_matrix(int n, int m, double scal, double **src_matrix, double **dest_matrix)
{
    int i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            dest_matrix[i][j]=scal*src_matrix[i][j];
        }
    }
}

void subtract_matrix(int n, int m, double **mat1, double **mat2, double **mat3)
{
    int i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            mat3[i][j]=mat1[i][j]-mat2[i][j];
        }
    }
}

double trace(int n, int m, double **matrix)
{
    int i=0, j=0;
    double tra=0.0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            if(i==j)
            {
                tra+=matrix[i][j];
            }
        }
    }
    return(tra);
}

long get_m_size(char *filename)
{
    FILE *ptsfile;
    char buf[256];
    long linecount=0;

    memset(buf, 0, sizeof(buf));
    ptsfile = fopen( filename, "r");
    if(ptsfile == NULL)
    {
        fprintf(stderr,"Error opening %s\r\n",filename);
        exit(-1);
    }
    // Count points
    while(fgets( buf, 128, ptsfile)!=NULL)
    {
        linecount++;
    }
    fclose(ptsfile);
    return(linecount);
}

void read_points(char *filename, double **pts_mat)
{
    FILE *ptsfile;
    char buf[256];
    long linecount=0;
    double x, y ,z;

    memset(buf, 0, sizeof(buf));
    ptsfile = fopen( filename, "r");
    if(ptsfile == NULL)
    {
        fprintf(stderr,"Error opening %s\r\n",filename);
        exit(-1);
    }
    linecount=0;
    while(fgets( buf, 128, ptsfile)!=NULL)
    {
        sscanf( buf, "%lf %lf %lf", &x, &y, &z);
        pts_mat[linecount][0]=x;
        pts_mat[linecount][1]=y;
        pts_mat[linecount][2]=z;
        linecount++;
    }
    printf("%s:\n", filename);
    plot_matrix(stdout, linecount, 3, pts_mat);
    printf("\n");
    fclose(ptsfile);
}

int main(int argc, char* argv[])
{

    char *src_pts_name = "";
    char *dest_pts_name = "";
    char *out_param_name = "";
    int n=3, m=0, m2=0;
    int k,l;
    double **src_mat=NULL;
    double **dest_mat=NULL;
    double **dest_mat_T=NULL;
    double **src_mat_T=NULL;
    double **E_mat=NULL;
    double **C_mat=NULL;
    double **C_mat_interm=NULL;
    double **D_mat_interm=NULL;
    double **P_mat=NULL;
    double *D_vec=NULL;
    double *T_vec=NULL;
    double *one_vec=NULL;
    double **D_mat=NULL;
    double **Q_mat=NULL;
    double **P_mat_T=NULL;
    double **R_mat=NULL;
    double trace1=0.0, trace2=0.0;
    double scal=0.0, ppm=0.0;
    FILE *outfile;

    printf("\n*******************************\n");
    printf(  "*      helmparms3d v%1.2f      *\n",VERS);
    printf(  "*   (c) U. Niethammer 2020    *\n");
    printf(  "*  http://helmparms3d.sf.net  *\n");
    printf(  "*******************************\n");
    if(argc < 4)
    {
        printf("Syntax: %s [src_pts_filename] [dest_pts_filename] [out_param_filename]\n\n\n",argv[0]);
        exit(1);
    }
    src_pts_name = argv[1];
    dest_pts_name = argv[2];
    out_param_name = argv[3];

    m = get_m_size(src_pts_name);
    m2 = get_m_size(dest_pts_name);
    if(m2 != m)
    {
        printf("Error, number of source and destination points is not equal!\n");
        exit(-1);
    }

    src_mat = matrix(m, m, src_mat);
    dest_mat = matrix(m, m, dest_mat);

    read_points(src_pts_name, src_mat);
    read_points(dest_pts_name, dest_mat);

    D_vec=vector(n, D_vec);

    E_mat=matrix(m, m, E_mat);
    P_mat=matrix(m, m, P_mat);
    D_mat=matrix(m, m, D_mat);
    Q_mat=matrix(m, m, Q_mat);
    P_mat_T=matrix(m, m, P_mat_T);
    R_mat=matrix(m, m, R_mat);
    dest_mat_T=matrix(m, m, dest_mat_T);
    C_mat=matrix(m, m, C_mat);
    C_mat_interm=matrix(m, m, C_mat_interm);
    src_mat_T=matrix(m, m, src_mat_T);
    D_mat_interm=matrix(m, m, D_mat_interm);

    transpose_matrix(m, m, dest_mat, dest_mat_T);
#if DEBUG
    printf("%s_T:\n",dest_pts_name);
    plot_matrix(stdout,  n, m, dest_mat_T);
#endif

    for(k=0; k<m; k++)
    {
        for(l=0; l<m; l++)
        {
            if(k!=l)
            {
                E_mat[k][l]=-1.0/(double)m;
            }
            else
            {
                E_mat[k][l]=1.0-1.0/(double)m;
            }
        }
    }
#if DEBUG
    printf("E:\n");
    plot_matrix(stdout,  m, m, E_mat);

    printf("dest_mat_T:\n");
    plot_matrix(stdout,  n, m, dest_mat_T);
#endif

    matmult(dest_mat_T, m, m, E_mat, m, m,  C_mat_interm, m, n);
#if DEBUG
    printf("C_interm:\n");
    plot_matrix(stdout,  n, m, C_mat_interm);
#endif

    matmult(C_mat_interm, n, m, src_mat, m, n,  C_mat, n, n);
#if DEBUG
    printf("C:\n");
    plot_matrix(stdout,  n, n, C_mat);
#endif

    copy_matrix(n,n,C_mat,P_mat);
#if DEBUG
    printf("P:\n");
    plot_matrix(stdout,  n, n, P_mat);
#endif

    //Given matrix C[m][n], m>=n, using svd decomposition C = P D Q' to get P[m][n], diag D[n] and Q[n][n].
    svd(n, n, C_mat, P_mat, D_vec, Q_mat);
    transpose_matrix(n, n, P_mat, P_mat_T);
#if DEBUG
    printf("P\n");
    plot_matrix(stdout,  n, n, P_mat);
    printf("P_T\n");
    plot_matrix(stdout,  n, n, P_mat_T);

    printf("D_vec\n");
    plot_vector(stdout,  n, D_vec);
#endif
    for(k=0; k<n; k++)
    {
        for(l=0; l<n; l++)
        {
            D_mat[k][l]=0.0;
            D_mat[l][l]=D_vec[l];

        }
    }
#if DEBUG
    printf("D\n");
    plot_matrix(stdout,  n, n, D_mat);
#endif

    matmult(Q_mat, n, n, P_mat_T, n, n,  R_mat, n, n);
#if DEBUG
    printf("R_trans:\n");
    plot_matrix(stdout, n, n, R_mat);
#endif

    matmult(C_mat, m, n, R_mat, n, m,  C_mat_interm, m, n);
#if DEBUG
    printf("C_interm:\n");
    plot_matrix(stdout,  n, n, C_mat_interm);
#endif
    trace1=trace(n,n,C_mat_interm);
#if DEBUG
    printf("\ntra=%lf\n\n",trace1);
#endif

    transpose_matrix(m, m, src_mat, src_mat_T);
#if DEBUG
    printf("%s_T:\n",src_pts_name);
    plot_matrix(stdout,  n, m, src_mat_T);
#endif

    init_matrix(m,m,C_mat);
    init_matrix(m,m,C_mat_interm);
    matmult(src_mat_T, m, m, E_mat, m, m,  C_mat_interm, n, n);
#if DEBUG
    printf("C_interm:\n");
    plot_matrix(stdout,  n, m, C_mat_interm);
#endif

    matmult(C_mat_interm, n, m, src_mat, m, n,  C_mat, n, n);
#if DEBUG
    printf("C:\n");
    plot_matrix(stdout,  n, n, C_mat);
#endif

    trace2=trace(n,n,C_mat);
#if DEBUG
    printf("\ntra=%lf\n\n",trace2);
#endif

    scal=trace1/trace2;
    ppm=scal-1.0;
#if DEBUG
    printf("\nscal = %10.10lf\nscal = %10.10lf ppm\n\n",scal, ppm);
#endif

    init_matrix(m,m,C_mat);
    init_matrix(m,m,C_mat_interm);

    matmult(src_mat, m, n, R_mat, n,m,  D_mat_interm, m, n);
#if DEBUG
    printf("C_mat_interm:\n");
    plot_matrix(stdout,  m, n, D_mat_interm);
#endif

    scal_matrix(m, n, scal, D_mat_interm, C_mat_interm);
#if DEBUG
    printf("C_mat_interm:\n");
    plot_matrix(stdout,  m, n, C_mat_interm);
#endif

    subtract_matrix(m, n, dest_mat, C_mat_interm, D_mat_interm);
#if DEBUG
    plot_matrix(stdout,  m, n, D_mat_interm);
#endif

    scal_matrix(m, n, 1.0/m, D_mat_interm, C_mat_interm);
#if DEBUG
    plot_matrix(stdout,  m, n, C_mat_interm);
#endif

    init_matrix(m,m,src_mat_T);
    transpose_matrix(m, m, C_mat_interm, src_mat_T);
#if DEBUG
    plot_matrix(stdout,  n, m, src_mat_T);
#endif

    T_vec=vector(m, T_vec);
    one_vec=vector(m, one_vec);
    for(k=0; k<m; k++)
    {
        one_vec[k]=1.0;
    }
    matrix_multiply(n, m, src_mat_T, one_vec, T_vec);
#if DEBUG
    printf("T:\n");
    plot_vector(stdout, 3, T_vec);
#endif

    outfile = fopen(out_param_name, "w");
    if(outfile == NULL)
    {
        printf("Error writing %s\r\n",out_param_name);
        exit(-1);
    }
    init_matrix(m,m,src_mat_T);
    transpose_matrix(m, m, R_mat, src_mat_T);
    plot_matrix(outfile, n, n, src_mat_T);
    printf("R =\n");
    fflush(stdout);
    plot_matrix(stdout, n, n, src_mat_T);
    printf("\n");
    fflush(stdout);
    plot_vector(outfile, 3, T_vec);
    printf("T =\n");
    fflush(stdout);
    plot_vector(stdout, 3, T_vec);
    printf("\n");
    fflush(stdout);
    fprintf(outfile, "%10.10lf\n", scal);
    printf("s = %10.10lf (= %10.10lf ppm)\n\n",scal, ppm);
    fflush(stdout);
    fclose(outfile);

    freevector(D_vec);
    freevector(T_vec);
    freevector(one_vec);

    freematrix(m, src_mat);
    freematrix(m, dest_mat);
    freematrix(m, E_mat);
    freematrix(m, P_mat);
    freematrix(m, D_mat);
    freematrix(m, Q_mat);
    freematrix(m, P_mat_T);
    freematrix(m, R_mat);
    freematrix(m, dest_mat_T);
    freematrix(m, C_mat);
    freematrix(m, C_mat_interm);
    freematrix(m, src_mat_T);
    freematrix(m, D_mat_interm);
    printf("\n...done\n");
    return(0);
}
