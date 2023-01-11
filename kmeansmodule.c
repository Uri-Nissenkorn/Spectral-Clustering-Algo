#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "kmeans.h"

/* Declaring Variables */
double *datapoints, *cents;

/* Converts python list of points to array of coordinates */
static double* getDataPoints(PyObject *dataPoints_py, int n, int d)
{
    p = 0;
    PyObject *singlePoint_py, *singleCoordinate_py;

    datapoints = (double*)malloc(n*d*sizeof(double));
    if (datapoints == NULL)
    {
        printf("An Error Has Occurred");
        exit(1);
    }

    /* Copies values from python list of lists */
    for (i = 0; i < n; i++)
    {
        singlePoint_py = PyList_GetItem(dataPoints_py, i);

        for (j = 0; j < d; j++, p++)
        {
            singleCoordinate_py = PyList_GetItem(singlePoint_py, j);
            datapoints[p] = PyFloat_AsDouble(singleCoordinate_py);
        }
    }

    return datapoints;
}


/* Converts python list of points to array of coordinates */
static double* getCentroids(PyObject *centroids_py, int k, int d)
{
    p = 0;
    PyObject *singlePoint_py, *singleCoordinate_py;

    cents = (double*)malloc(k*d*sizeof(double));
    if (datapoints == NULL)
    {
        printf("An Error Has Occurred");
        free(datapoints);
        exit(1);
    }

    /* Copies values from python list of lists */
    for (i = 0; i < k; i++)
    {
        singlePoint_py = PyList_GetItem(centroids_py, i);

        for (j = 0; j < d; j++, p++)
        {
            singleCoordinate_py = PyList_GetItem(singlePoint_py, j);
            cents[p] = PyFloat_AsDouble(singleCoordinate_py);
        }
    }

    return cents;
}


static PyObject *kmeans(PyObject *self, PyObject *args)
{
    int k, n, d, max_iter;
    double eps;

    PyObject *dataPoints_py, *centroids_py;

    /* Terminates invalid parameters from kmeans_pp.py */
    if (!PyArg_ParseTuple(args, "iiiidOO:kmeans", &k, &n, &d, &max_iter, &eps, &dataPoints_py, &centroids_py))
        return NULL;
    if (!PyList_Check(dataPoints_py) || !PyList_Check(centroids_py))
        return NULL;

    /* Initializes variables */
    datapoints = getDataPoints(dataPoints_py, n, d);
    cents = getCentroids(centroids_py, k ,d);

    double *centroids =  Kmeans(k, n, d, max_iter, eps, datapoints, cents);

    PyObject* edges = PyList_New(k);
    int index = 0;
    for (i = 0; i < k*d; i+=d){
        PyObject* row = PyList_New(d);
        for (j = 0; j < d; j++){
            PyObject* edge = Py_BuildValue("d", centroids[i+j]);
            PyList_SetItem(row, j, edge);
        }
        PyList_SetItem(edges, index++, row);
    }

    if (centroids != NULL)
        free(centroids);

    return Py_BuildValue("{s:O}", "edges", edges);
}

static PyMethodDef capiMethod[] = {
    {"fit",                                                    /* the Python method name that will be used */
     (PyCFunction)kmeans,                                      /* the C-function that implements the Python function and returns static PyObject*  */
     METH_VARARGS,                                             /* flags indicating parameters accepted for this function */
     PyDoc_STR("kmeans algorithm for given data of vectors")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}                                      /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "kmeans_c", /* name of module */
    NULL,       /* module documentation, may be NULL */
    -1,         /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    capiMethod  /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC
PyInit_kmeans_c(void)
{
    return PyModule_Create(&moduledef);
}
