#ifndef PYTHON_BINDINGS_INCLUDE_H
#define PYTHON_BINDINGS_INCLUDE_H

#include "Python.h"
#include "structmember.h"
#include "exception.hpp"
#include "arch.hpp"
#include "database.hpp"
#include "compiler.hpp"

/* -------------------------------------------------
 *                     Utils
 * ------------------------------------------------- */

PyObject* create_class(PyObject* name, PyObject* bases, PyObject* dict);

/* --------------------------------------------------
 *                   Arch
 *  -------------------------------------------------- */

void init_arch(PyObject* module);

/* --------------------------------------------------
 *                   Ropium
 *  -------------------------------------------------- */

typedef struct{
    PyObject_HEAD
    Arch* arch;
    GadgetDB* gadget_db;
    ROPCompiler* compiler;
} ROPium_Object;
PyObject* get_ROPium_Type();
PyObject* ropium_ROPium(PyObject* self, PyObject* args);
#define as_ropium_object(x)  (*((ROPium_Object*)x))

#endif
