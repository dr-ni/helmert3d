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

static void print_vector(FILE *target, size_t dim,double *vector)
{
    size_t count;
    for(count=0; count<dim; count++)
    {
        fprintf(target, "%10.10lf ",vector[count]);
    }
    fprintf(target, "\n");
}

static void print_matrix(FILE *target, size_t b,size_t h,double **matrix_a)
{
    size_t i=0;
    size_t j=0;
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
static void matrix_multiply(size_t h,size_t b,double ** matrix,double *invector,double *outvector)
{
    size_t count;
    size_t count1;
    for(count=0; count<h; count++)outvector[count]=0.0;
    for(count=0; count<h; count++)
    {
        for(count1=0; count1<b; count1++)
        {
            outvector[count]+=matrix[count][count1]*invector[count1];
        }
    }
}

static void matmult(double **mat1, size_t m1, size_t n1, double **mat2, /*@unused@*/size_t m2, size_t n2, double **mat3, /*@unused@*/size_t m3, /*@unused@*/size_t n3)
{
    size_t i=0, j=0, k=0;
    double sum=0.0;
    if (m1 != n2)
    {
        fprintf(stderr,"Matrix m1 cannot be multiplied to m2.\n");
        exit(EXIT_FAILURE);
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

static void transpose_matrix(size_t n, size_t m, double **src_matrix, double **dest_matrix)
{
    size_t i=0, j=0;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            dest_matrix[i][j]=src_matrix[j][i];
        }
    }
}

static void init_matrix(size_t n, size_t m, double **matrix)
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

static void copy_matrix(size_t n, size_t m, double **src_matrix, double **dest_matrix)
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

static void scal_matrix(size_t n, size_t m, double scal, double **src_matrix, double **dest_matrix)
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

static void subtract_matrix(size_t n, size_t m, double **mat1, double **mat2, double **mat3)
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
    print_matrix(stdout, linecount, 3, pts_mat);
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
    fprintf(stdout,"%s_T:\n",dest_pts_name);
    print_matrix(stdout,  n, m, dest_mat_T);
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
    print_matrix(stdout,  m, m, E_mat);

    fprintf(stdout,"dest_mat_T:\n");
    print_matrix(stdout,  n, m, dest_mat_T);
#endif

    matmult(dest_mat_T, m, m, E_mat, m, m,  C_mat_interm, m, n);
#if DEBUG
    fprintf(stdout,"C_interm:\n");
    print_matrix(stdout,  n, m, C_mat_interm);
#endif

    matmult(C_mat_interm, n, m, src_mat, m, n,  C_mat, n, n);
#if DEBUG
    fprintf(stdout,"C:\n");
    print_matrix(stdout,  n, n, C_mat);
#endif

    copy_matrix(n,n,C_mat,P_mat);
#if DEBUG
    fprintf(stdout,"P:\n");
    print_matrix(stdout,  n, n, P_mat);
#endif

    //Given matrix C[m][n], m>=n, using svd decomposition C = P D Q' to get P[m][n], diag D[n] and Q[n][n].
    svd((long long) n, (long long)n, C_mat, P_mat, D_vec, Q_mat);
    transpose_matrix(n, n, P_mat, P_mat_T);
#if DEBUG
    fprintf(stdout,"P\n");
    print_matrix(stdout,  n, n, P_mat);
    fprintf(stdout,"P_T\n");
    print_matrix(stdout,  n, n, P_mat_T);

    fprintf(stdout,"D_vec\n");
    print_vector(stdout,  n, D_vec);
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
    print_matrix(stdout,  n, n, D_mat);
#endif

    matmult(Q_mat, n, n, P_mat_T, n, n,  R_mat, n, n);
#if DEBUG
    fprintf(stdout,"R_trans:\n");
    print_matrix(stdout, n, n, R_mat);
#endif

    matmult(C_mat, m, n, R_mat, n, m,  C_mat_interm, m, n);
#if DEBUG
    fprintf(stdout,"C_interm:\n");
    print_matrix(stdout,  n, n, C_mat_interm);
#endif
    trace1=trace(n,n,C_mat_interm);
#if DEBUG
    fprintf(stdout,"\ntra=%lf\n\n",trace1);
#endif

    transpose_matrix(m, m, src_mat, src_mat_T);
#if DEBUG
    fprintf(stdout,"%s_T:\n",src_pts_name);
    print_matrix(stdout,  n, m, src_mat_T);
#endif

    init_matrix(m,m,C_mat);
    init_matrix(m,m,C_mat_interm);
    matmult(src_mat_T, m, m, E_mat, m, m,  C_mat_interm, n, n);
#if DEBUG
    fprintf(stdout,"C_interm:\n");
    print_matrix(stdout,  n, m, C_mat_interm);
#endif

    matmult(C_mat_interm, n, m, src_mat, m, n,  C_mat, n, n);
#if DEBUG
    fprintf(stdout,"C:\n");
    print_matrix(stdout,  n, n, C_mat);
#endif

    trace2=trace(n,n,C_mat);
#if DEBUG
    fprintf(stdout,"\ntra=%lf\n\n",trace2);
#endif

    scal=trace1/trace2;
    ppm=(scal-1.0)*1000000.0;
#if DEBUG
    fprintf(stdout,"\nscal = %10.10lf\nscal = %10.10lf ppm\n\n",scal, ppm);
#endif

    init_matrix(m,m,C_mat);
    init_matrix(m,m,C_mat_interm);

    matmult(src_mat, m, n, R_mat, n,m,  D_mat_interm, m, n);
#if DEBUG
    fprintf(stdout,"C_mat_interm:\n");
    print_matrix(stdout,  m, n, D_mat_interm);
#endif

    scal_matrix(m, n, scal, D_mat_interm, C_mat_interm);
#if DEBUG
    fprintf(stdout,"C_mat_interm:\n");
    print_matrix(stdout,  m, n, C_mat_interm);
#endif

    subtract_matrix(m, n, dest_mat, C_mat_interm, D_mat_interm);
#if DEBUG
    print_matrix(stdout,  m, n, D_mat_interm);
#endif

    scal_matrix(m, n, 1.0/m, D_mat_interm, C_mat_interm);
#if DEBUG
    print_matrix(stdout,  m, n, C_mat_interm);
#endif

    init_matrix(m,m,src_mat_T);
    transpose_matrix(m, m, C_mat_interm, src_mat_T);
#if DEBUG
    print_matrix(stdout,  n, m, src_mat_T);
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
    print_vector(stdout, 3, T_vec);
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
    
    init_matrix(m,m,src_mat_T);
    transpose_matrix(m, m, R_mat, src_mat_T);
    fprintf(stdout,"R =\n");

    print_matrix(stdout, n, n, src_mat_T);
    fprintf(stdout,"\n");

    fprintf(stdout,"T =\n");

    print_vector(stdout, 3, T_vec);
    fprintf(stdout,"\n");

    fprintf(stdout,"s = %10.10lf (= %10.10lf ppm)\n\n",scal, ppm);

    if(argc < 4)
    {
        fprintf(stdout,"Results matrix:\n");
    }
    print_matrix(outfile, n, n, src_mat_T);
    print_vector(outfile, 3, T_vec);
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

    fprintf(stdout,"...done\n");
    return(0);
}
