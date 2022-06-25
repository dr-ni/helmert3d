/* Copyright (C) 2021 U. Niethammer,  http://helmparms3d.sourceforge.net/
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
#include "svdm.h"

static void freematrix(size_t m, /*@only@*/ /*@out@*/ /*@null@*/ double ** u)
{
    size_t i=0;
    for(i = 0; i < m; i++)
    {
        free(u[i]);
    }
    free(u);
}

static void freevector(/*@only@*/ /*@out@*/ /*@null@*/ double * u)
{
    free(u);
}

static double ** matrix(size_t m, size_t n, /*@null@*/double ** u)
{
    size_t i=0;
    u = calloc(m, sizeof(*u));
    if(u == NULL)
    {
        fprintf(stderr,"Error memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < m; i++)
    {
        u[i] = calloc(n * sizeof(*u[0]),sizeof(double *));
        if(u[i] == NULL)
        {
            fprintf(stderr,"Error memory allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    return u;
}

static double * vector(size_t m, /*@null@*/double * u)
{
    u = calloc(m, sizeof(*u));
    if(u == NULL)
    {
        fprintf(stderr,"Error memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return u;
}

static void vector_print(FILE *target, size_t n,double *vector)
{
    size_t i=0;
    for(i=0; i<n; i++)
    {
        fprintf(target, "%10.10lf ",vector[i]);
    }
    fprintf(target, "\n");
}

static void matrix_print(FILE *target, size_t n,size_t m,double **matrix_a)
{
    size_t i=0, j=0;
    for(i=0; i<n; i++)
    {
        for(j=0; j<m; j++)
        {
            fprintf(target, "%10.10lf ", matrix_a[i][j]);
        }
        fprintf(target, "\n");
    }
}

//Matrix mit Vektor multiplizieren
static void matrix_multiply(size_t n,size_t m,double ** matrix,double *invector,double *outvector)
{
    size_t i=0, j=0;
    for(i=0; i<n; i++)
    {
        outvector[i]=0.0;
        for(j=0; j<m; j++)
        {
            outvector[i]+=matrix[i][j]*invector[j];
        }
    }
}

static void matmult(size_t n, size_t m, size_t m2, double **mat1, double **mat2, double **mat3)
{
    size_t i=0, j=0, k=0;
    double sum=0.0;
    for(i=0; i<n; i++)
    {
        for(j=0; j<m2; j++)
        {
            sum=0.0;
            for(k=0; k<m; k++)
            {
                sum+=mat1[i][k]*mat2[k][j];
            }
            mat3[i][j]=sum;
        }
    }
}

static void matrix_transpose(size_t n, size_t m, double **src_matrix, double **dest_matrix)
{
    size_t i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            dest_matrix[j][i]=src_matrix[i][j];
        }
    }
}

static void matrix_init(size_t n, size_t m, double **matrix)
{
    size_t i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            matrix[i][j]=0.0;
        }
    }
}

static void matrix_copy(size_t n, size_t m, double **src_matrix, double **dest_matrix)
{
    size_t i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            dest_matrix[i][j]=src_matrix[i][j];
        }
    }
}

static void matrix_scal(size_t n, size_t m, double scal, double **src_matrix, double **dest_matrix)
{
    size_t i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            dest_matrix[i][j]=scal*src_matrix[i][j];
        }
    }
}

static void matrix_subtract(size_t n, size_t m, double **mat1, double **mat2, double **mat3)
{
    size_t i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            mat3[i][j]=mat1[i][j]-mat2[i][j];
        }
    }
}

static double trace(size_t n, size_t m, double **matrix)
{
    size_t i=0, j=0;
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

static size_t get_m_size(char *filename)
{
    FILE *ptsfile;
    char buf[256];
    size_t linecount=0;

    memset(buf, 0, sizeof(buf));
    ptsfile = fopen( filename, "r");
    if(ptsfile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",filename);
        exit(EXIT_FAILURE);
    }
    // Count points
    while(fgets( buf, 128, ptsfile)!=NULL)
    {
        if( strcmp(buf,"\n") != 0 && strcmp(buf,"\n") != 0 && strcmp(buf,"\0") != 0)
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

static void read_points(char *filename, double **pts_mat)
{
    FILE *ptsfile;
    char buf[256];
    size_t linecount=0;
    double x, y ,z;
    int stat=0;

    memset(buf, 0, sizeof(buf));
    ptsfile = fopen( filename, "r");
    if(ptsfile == NULL)
    {
        fprintf(stderr,"Error opening %s\n",filename);
        exit(EXIT_FAILURE);
    }
    linecount=0;
    while(fgets(buf, 128, ptsfile)!=NULL)
    {
        stat=sscanf(buf, "%lf %lf %lf", &x, &y, &z);
        if(stat != 3)
        {
            fprintf(stderr,"Error wrong data format in %s\n",filename);
            exit(EXIT_FAILURE);
        }
        pts_mat[linecount][0]=x;
        pts_mat[linecount][1]=y;
        pts_mat[linecount][2]=z;
        linecount++;
    }
    printf("%s:\n", filename);
    matrix_print(stdout, linecount, 3, pts_mat);
    fprintf(stdout,"\n");
    (void)fclose(ptsfile);
}

int main(int argc, char* argv[])
{

    char *src_pts_name = "";
    char *dest_pts_name = "";
    char *out_param_name = "";
    size_t n=3, m=0, m2=0;
    size_t k,l;
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
    double rx=0.0, ry=0.0, rz=0.0;
    FILE *outfile;

    fprintf(stdout,"\n*******************************\n");
    fprintf(stdout,  "*      helmparms3d v%s     *\n",VERS);
    fprintf(stdout,  "*   (c) %s %s    *\n",C_AUTHOR,C_YEAR);
    fprintf(stdout,  "*******************************\n");
    if(argc < 3)
    {
        fprintf(stdout,"\nSyntax: %s xyz_src_infilename xyz_dest_infilename [helmert_param_outfilename]\n\n",argv[0]);
        fprintf(stderr,"helmert parameter file format:\n");
        fprintf(stderr," r11 r12 r13\n r21 r22 r23\n r31 r32 r33\n tx ty tz\n s\n\n");
        fprintf(stderr,"xyz data file format:\n");
        fprintf(stderr," X[1] Y[1] Z[1]\n ..   ..   ..\n ..   ..   ..\n X[n] Y[n] Z[n]\n\n");
        exit(EXIT_FAILURE);
    }
    src_pts_name = argv[1];
    dest_pts_name = argv[2];

    m = get_m_size(src_pts_name);
    m2 = get_m_size(dest_pts_name);
    if(m2 != m)
    {
        fprintf(stderr,"Error, number of source and destination points is not equal!\n");
        exit(EXIT_FAILURE);
    }

    if((m < 3) || (m2 < 3))
    {
        fprintf(stderr,"Error, number of point correspondences is too small!\n");
        exit(EXIT_FAILURE);
    }

    src_mat = matrix(m, n, src_mat);
    dest_mat = matrix(m, n, dest_mat);

    read_points(src_pts_name, src_mat);
    read_points(dest_pts_name, dest_mat);

    D_vec=vector(n, D_vec);

    E_mat=matrix(m, m, E_mat);
    P_mat=matrix(n, n, P_mat);
    D_mat=matrix(n, n, D_mat);
    Q_mat=matrix(n, n, Q_mat);
    P_mat_T=matrix(n, n, P_mat_T);
    R_mat=matrix(n, n, R_mat);
    dest_mat_T=matrix(n, m, dest_mat_T);
    C_mat=matrix(n, n, C_mat);
    C_mat_interm=matrix(m, m, C_mat_interm);
    src_mat_T=matrix(n, m, src_mat_T);
    D_mat_interm=matrix(m, m, D_mat_interm);

    matrix_transpose(m, n, dest_mat, dest_mat_T);
#if DEBUG
    fprintf(stdout,"%s_T:\n",dest_pts_name);
    matrix_print(stdout, n, m, dest_mat_T);
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
    fprintf(stdout,"E:\n");
    matrix_print(stdout, m, m, E_mat);

    fprintf(stdout,"dest_mat_T:\n");
    matrix_print(stdout, n, m, dest_mat_T);
#endif

    matmult(n, m, m, dest_mat_T, E_mat, C_mat_interm);
#if DEBUG
    fprintf(stdout,"C_interm:\n");
    matrix_print(stdout, n, m, C_mat_interm);
#endif

    matmult(n, m, n, C_mat_interm, src_mat, C_mat);
#if DEBUG
    fprintf(stdout,"C:\n");
    matrix_print(stdout, n, n, C_mat);
#endif

    matrix_copy(n, n, C_mat, P_mat);
#if DEBUG
    fprintf(stdout,"P:\n");
    matrix_print(stdout, n, n, P_mat);
#endif

    //Given matrix C[m][n], m>=n, using svd decomposition C = P D Q' to get P[m][n], diag D[n] and Q[n][n].
    svd((long long) n, (long long)n, C_mat, P_mat, D_vec, Q_mat);
    matrix_transpose(n, n, P_mat, P_mat_T);
#if DEBUG
    fprintf(stdout,"P\n");
    matrix_print(stdout, n, n, P_mat);
    fprintf(stdout,"P_T\n");
    matrix_print(stdout, n, n, P_mat_T);

    fprintf(stdout,"D_vec\n");
    vector_print(stdout, n, D_vec);
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
    fprintf(stdout,"D\n");
    matrix_print(stdout, n, n, D_mat);
#endif

    matmult(n, n, n, Q_mat, P_mat_T, R_mat);
#if DEBUG
    fprintf(stdout,"R_trans:\n");
    matrix_print(stdout, n, n, R_mat);
#endif

    matmult(n, n, n, C_mat, R_mat, C_mat_interm);
#if DEBUG
    fprintf(stdout,"C_interm:\n");
    matrix_print(stdout, n, n, C_mat_interm);
#endif
    trace1=trace(n, n, C_mat_interm);
#if DEBUG
    fprintf(stdout,"\ntra=%lf\n\n",trace1);
#endif

    matrix_transpose(m, n, src_mat, src_mat_T);
#if DEBUG
    fprintf(stdout,"%s_T:\n",src_pts_name);
    matrix_print(stdout, n, m, src_mat_T);
#endif

    matrix_init(n, n, C_mat);
    matrix_init(m, m, C_mat_interm);
    matmult(n, m, m, src_mat_T, E_mat, C_mat_interm);
#if DEBUG
    fprintf(stdout,"C_interm:\n");
    matrix_print(stdout, n, m, C_mat_interm);
#endif

    matmult(n, m, n, C_mat_interm, src_mat, C_mat);
#if DEBUG
    fprintf(stdout,"C:\n");
    matrix_print(stdout, n, n, C_mat);
#endif

    trace2=trace(n, n, C_mat);
#if DEBUG
    fprintf(stdout,"\ntra=%lf\n\n",trace2);
#endif

    scal=trace1/trace2;
    ppm=(scal-1.0)*1000000.0;
#if DEBUG
    fprintf(stdout,"\nscal = %10.10lf\nscal = %10.10lf ppm\n\n",scal, ppm);
#endif

    matrix_init(n, n, C_mat);
    matrix_init(m, m, C_mat_interm);

    matmult(m, n, n, src_mat, R_mat, D_mat_interm);
#if DEBUG
    fprintf(stdout,"C_mat_interm:\n");
    matrix_print(stdout, m, n, D_mat_interm);
#endif

    matrix_scal(m, n, scal, D_mat_interm, C_mat_interm);
#if DEBUG
    fprintf(stdout,"C_mat_interm:\n");
    matrix_print(stdout, m, n, C_mat_interm);
#endif

    matrix_subtract(m, n, dest_mat, C_mat_interm, D_mat_interm);
#if DEBUG
    matrix_print(stdout, m, n, D_mat_interm);
#endif

    matrix_scal(m, n, 1.0/m, D_mat_interm, C_mat_interm);
#if DEBUG
    matrix_print(stdout, m, n, C_mat_interm);
#endif

    matrix_init(n, m, src_mat_T);
    matrix_transpose(m, n, C_mat_interm, src_mat_T);
#if DEBUG
    matrix_print(stdout, n, m, src_mat_T);
#endif

    T_vec=vector(m, T_vec);
    one_vec=vector(m, one_vec);
    for(k=0; k<m; k++)
    {
        one_vec[k]=1.0;
    }
    matrix_multiply(n, m, src_mat_T, one_vec, T_vec);
#if DEBUG
    fprintf(stdout,"T:\n");
    vector_print(stdout, 3, T_vec);
#endif

    if(argc > 3)
    {
        out_param_name = argv[3];
        outfile = fopen(out_param_name, "w");
    }
    else
        outfile = stdout;
    if(outfile == NULL)
    {
        fprintf(stderr,"Error writing %s\n",out_param_name);
        exit(EXIT_FAILURE);
    }

    matrix_init(n, m, src_mat_T);
    matrix_transpose(n, n, R_mat, src_mat_T);
    fprintf(stdout,"R =\n");

    matrix_print(stdout, n, n, src_mat_T);
    fprintf(stdout,"\n");

    fprintf(stdout,"T =\n");

    vector_print(stdout, 3, T_vec);
    fprintf(stdout,"\n");

    fprintf(stdout,"s = %10.10lf (= %10.10lf ppm)\n\n",scal, ppm);

    rx = 0.5 * (src_mat_T[2][1] - src_mat_T[1][2]) * RAD2SEC;
    ry = 0.5 * (src_mat_T[0][2] - src_mat_T[2][0]) * RAD2SEC;
    rz = 0.5 * (src_mat_T[1][0] - src_mat_T[0][1]) * RAD2SEC;
    fprintf(stdout,"Proj string:\n");
    fprintf(stdout,"+towgs84=%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n\n", T_vec[0], T_vec[1], T_vec[2], rx, ry, rz, ppm);

    if(argc < 4)
    {
        fprintf(stdout,"Results matrix:\n");
    }
    matrix_print(outfile, n, n, src_mat_T);
    vector_print(outfile, 3, T_vec);
    fprintf(outfile, "%10.10lf\n", scal);
    if(argc > 3)
    {
        fprintf(stdout,"Results written to %s\n", out_param_name);
        (void)fclose(outfile);
    }
    freevector(D_vec);
    freevector(T_vec);
    freevector(one_vec);

    freematrix(m, src_mat);
    freematrix(m, dest_mat);
    freematrix(m, E_mat);
    freematrix(n, P_mat);
    freematrix(n, D_mat);
    freematrix(n, Q_mat);
    freematrix(n, P_mat_T);
    freematrix(n, R_mat);
    freematrix(n, dest_mat_T);
    freematrix(n, C_mat);
    freematrix(m, C_mat_interm);
    freematrix(n, src_mat_T);
    freematrix(m, D_mat_interm);

    fprintf(stdout,"...done\n");
    return(0);
}
