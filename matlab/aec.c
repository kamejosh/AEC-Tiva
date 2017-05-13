#include <matrix.h>
#include <mex.h>   

/* Definitions to keep compatibility with earlier versions of ML */
#ifndef MWSIZE_MAX
typedef int mwSize;
typedef int mwIndex;
typedef int mwSignedIndex;

#if (defined(_LP64) || defined(_WIN64)) && !defined(MX_COMPAT_32)
/* Currently 2^48 based on hardware limitations */
# define MWSIZE_MAX    281474976710655UL
# define MWINDEX_MAX   281474976710655UL
# define MWSINDEX_MAX  281474976710655L
# define MWSINDEX_MIN -281474976710655L
#else
# define MWSIZE_MAX    2147483647UL
# define MWINDEX_MAX   2147483647UL
# define MWSINDEX_MAX  2147483647L
# define MWSINDEX_MIN -2147483647L
#endif
#define MWSIZE_MIN    0UL
#define MWINDEX_MIN   0UL
#endif

void cancelEcho(double *near, double *far, double *out, int dim, double *sinus)
{
    int i;
    double mu = 0.0005; //stepsize
    double alfa = 0.0008;
    double c = 0.001; //constant to prevent large steps
    double weight = 0.0; //filter weights
    double y = 0.0;
    double down;

    for(i=0;i<dim;i++)
    {
        down = c + (far[i] * far[i]);
        mu = alfa/down;
        
        y = weight * far[i];

        out[i] = near[i] - y;

        weight = weight + mu * out[i] * far[i];

//         mexPrintf("iteration %d - down %f - mu %f - filter %f - weight %f - far %f - near %f - out %f - sinus %f\n", 
//                     i, down, mu, y, weight, far[i], near[i], out[i], sinus[i]);
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
//declare variables
    mxArray *near_in_m, *far_in_m, *out_out_m, *sinus_in_m;
    const mwSize *dims;
    double *near, *far, *out, *sinus;
    int dimx, dimy, numdims;

//associate inputs
    near_in_m = mxDuplicateArray(prhs[0]);
    far_in_m = mxDuplicateArray(prhs[1]);
    sinus_in_m = mxDuplicateArray(prhs[2]);

//figure out dimensions
    dims = mxGetDimensions(prhs[0]);
    numdims = mxGetNumberOfDimensions(prhs[0]);
    dimy = (int)dims[0]; dimx = (int)dims[1];

//associate outputs
    out_out_m = plhs[0] = mxCreateDoubleMatrix(dimy,dimx,mxREAL);

//associate pointers
    near = mxGetPr(a_in_m);
    far = mxGetPr(b_in_m);
    out = mxGetPr(c_out_m);
    sinus = mxGetPr(d_in_m);

//do something
    cancelEcho(near, far, out, dimx, sinus);

    return;
}