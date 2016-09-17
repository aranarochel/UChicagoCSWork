#ifndef _MATRIX_H
#define _MATRIX_H

typedef struct matrix matrix;

struct matrix {
  int n_rows;
  int n_cols;
  float *entries;
};

/* The following matrix has 2 rows and 3 columns:

   1.0 3.0 5.0
   7.1 8.1 9.1

   Store matrices in row-major order, which means,
   row by row, top to bottom. In the example case,
   that would be

   1.0 3.0 5.0 7.1 8.1 9.1 
*/

/* matrix_zero : int, int -> matrix*    */
/* - build a matrix of given dimension, */
/*   all entries 0.0                    */
matrix *matrix_zero(int n_rows, int n_cols);

/* matrix_read : matrix*, int, int -> float      */
/* - read the item at row i, column j            */
/* - error if row, column are outside the matrix */
float matrix_read(matrix *m, int i, int j);

/* matrix_write : matrix*, int, int, float -> [void] */
/* - write the float values to the location          */
/* - error if row, column are outside the matrix     */
void matrix_write(matrix *m, int i, int j, float x);

/* matrix_build : int, int, (int,int -> float) -> matrix*    */
/* - build a matrix whose entries are computed from row, col */
/*   by given function                                       */
matrix *matrix_build(int n_rows, int n_cols, float(*f)(int,int));

/* matrix_ident : int -> matrix*                 */
/* - construct the identity matrix of given size */
matrix *matrix_ident(int dim);

/* matrix_add : matrix*, matrix* -> matrix* */
/* - add two matrices non-destructively     */
/*   (i.e., construct a new matrix)         */
/* - error if matrices' dimensions differ   */
matrix *matrix_add(matrix *m, matrix *n);

/* matrix_mult : matrix*, matrix* -> matrix*                  */
/* - multiply two matrices, constructing a new matrix         */
/* - error if dimensions don't allow matrix multiplication    */
/* - note: this is matrix multiplication, not pairwise product */
matrix *matrix_mult(matrix *m, matrix *n);

/* matrix_transpose : matrix* -> matrix* */
matrix *matrix_transpose(matrix *m);

/* matrix_show : matrix* -> [void] */
void matrix_show(matrix *m);

/* matrix_free : matrix* -> [void] */
void matrix_free(matrix *m);

#endif /* _MATRIX_H */


