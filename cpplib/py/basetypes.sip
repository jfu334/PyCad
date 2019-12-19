//
// type conversion methods
//

%MappedType std::string
{
	%TypeHeaderCode
	#include <string>
	%End

	%ConvertFromTypeCode
		// convert an std::string to a Python (unicode) string
		PyObject* newstring;
		newstring = PyUnicode_DecodeUTF8(sipCpp->c_str(), sipCpp->length(), NULL);
		if(newstring == NULL) {
			PyErr_Clear();
			newstring = PyBytes_FromString(sipCpp->c_str());
		}
		return newstring;
	%End

	%ConvertToTypeCode
		// Allow a Python string (or a unicode string) whenever a string is
		// expected.
		// If argument is a Unicode string, just decode it to UTF-8
		// If argument is a Python string, assume it's UTF-8
		if (sipIsErr == NULL)
			return (PyBytes_Check(sipPy) || PyUnicode_Check(sipPy));

		if (sipPy == Py_None)
		{
			*sipCppPtr = new std::string;
			return 1;
		}

		if (PyUnicode_Check(sipPy))
		{
			PyObject* s = PyUnicode_AsEncodedString(sipPy, "UTF-8", "");
			*sipCppPtr = new std::string(PyBytes_AS_STRING(s));
			Py_DECREF(s);
			return 1;
		}

		if (PyBytes_Check(sipPy))
		{
			*sipCppPtr = new std::string(PyBytes_AS_STRING(sipPy));
			return 1;
		}
		return 0;
	%End
};

template<TYPE>
%MappedType std::vector<TYPE>
{
	%TypeHeaderCode
	#include <vector>
	%End

	%ConvertFromTypeCode
		PyObject *l = PyList_New(sipCpp -> size());

		// Create the Python list of the correct length.
		if(!l) return NULL;

		// Go through each element in the C++ instance and convert it to a
		// wrapped P2d.
		for (int i = 0; i < (int)sipCpp->size(); ++i) {
			TYPE *cpp = new TYPE(sipCpp->at(i));
			PyObject *pobj = sipConvertFromInstance(cpp, sipClass_TYPE, sipTransferObj);

			// Get the Python wrapper for the Type instance, creating a new
			// one if necessary, and handle any ownership transfer.
			if (!pobj) {
				// There was an error so garbage collect the Python list.
				Py_DECREF(l);
				return NULL;
			}

			// Add the wrapper to the list.
			PyList_SET_ITEM(l, i, pobj);
		}

		// Return the Python list.
		return l;
	%End

	%ConvertToTypeCode
		// Check if type is compatible
		if (!sipIsErr) {
			// Must be any iterable
			PyObject *i = PyObject_GetIter(sipPy);
			bool iterable = (i != NULL);
			Py_XDECREF(i);
			return iterable;
		}

		// Iterate over the object
		PyObject *iterator = PyObject_GetIter(sipPy);
		PyObject *item;

		std::vector<TYPE> *V = new std::vector<TYPE>();

		while ((item = PyIter_Next(iterator)))
		{
			if (!sipCanConvertToInstance(item, sipClass_TYPE, SIP_NOT_NONE)) {
				PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to TYPE");
				*sipIsErr = 1;
				break;
			}

			int state;
			TYPE* p = reinterpret_cast<TYPE*>(
				sipConvertToInstance(item, sipClass_TYPE, 0, SIP_NOT_NONE, &state, sipIsErr));

			if (!*sipIsErr)
				V->push_back(*p);

			sipReleaseInstance(p, sipClass_TYPE, state);
			Py_DECREF(item);
		}

		Py_DECREF(iterator);

		if (*sipIsErr) {
			delete V;
			return 0;
		}

		*sipCppPtr = V;
		return sipGetState(sipTransferObj);
	%End
};

template<TYPE>
%MappedType std::vector<TYPE*>
{
	%TypeHeaderCode
	#include <vector>
	%End

	%ConvertFromTypeCode
		PyObject *l = PyList_New(sipCpp->size());

		// Create the Python list of the correct length.
		if(!l) return NULL;

		// Go through each element in the C++ instance and convert it to a
		// wrapped P2d.
		for (int i = 0; i < (int)sipCpp->size(); ++i) {
			TYPE *cpp =sipCpp->at(i);
			PyObject *pobj = sipConvertFromInstance(cpp, sipClass_TYPE, sipTransferObj);

			// Get the Python wrapper for the Type instance, creating a new
			// one if necessary, and handle any ownership transfer.
			if (!pobj) {
				// There was an error so garbage collect the Python list.
				Py_DECREF(l);
				return NULL;
			}

			// Add the wrapper to the list.
			PyList_SET_ITEM(l, i, pobj);
		}

		// Return the Python list.
		return l;
	%End

	%ConvertToTypeCode
		// Check if type is compatible
		if (!sipIsErr) {
			// Must be any iterable
			PyObject *i = PyObject_GetIter(sipPy);
			bool iterable = (i != NULL);
			Py_XDECREF(i);
			return iterable;
		}

		// Iterate over the object
		PyObject *iterator = PyObject_GetIter(sipPy);
		PyObject *item;

		std::vector<TYPE*> *V = new std::vector<TYPE*>();

		while ((item = PyIter_Next(iterator)))
		{
			if (!sipCanConvertToInstance(item, sipClass_TYPE, SIP_NOT_NONE)) {
				PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to TYPE");
				*sipIsErr = 1;
				break;
			}

			int state;
			TYPE* p=reinterpret_cast<TYPE*>(
				sipConvertToInstance(item, sipClass_TYPE, 0, SIP_NOT_NONE, &state, sipIsErr));

			if (!*sipIsErr)
				V->push_back(p);

			// TODO?
			//sipReleaseInstance(p, sipClass_TYPE, state);
			Py_DECREF(item);
		}

		Py_DECREF(iterator);

		if (*sipIsErr) {
			delete V;
			return 0;
		}

		*sipCppPtr = V;
		return sipGetState(sipTransferObj);
	%End
};

/*
 * Nested vector
 */

/* NOT WORKING
template<TYPE>
%MappedType std::vector<std::vector<TYPE> >
{
	%TypeHeaderCode
	#include <vector>
	%End

	%ConvertFromTypeCode

		// Create the Python list of the correct length.
		PyObject *l=PyList_New(sipCpp->size());
		if(!l) return nullptr;
		
		for(size_t i=0;i<sipCpp->size();i++)
		{
			PyObject* li=PyList_New(sipCpp[i].size());
			if(!li) return nullptr;
			PyList_SET_ITEM(l, i, li);
		}
		
		// copy elements
		for(size_t i=0;i<sipCpp->size();i++)
		for(size_t j=0;j<sipCpp[i].size();j++)
		{
			TYPE* cpp=new TYPE((*sipCpp)[i][j]);
			
			PyObject *pobj=sipConvertFromInstance(cpp, sipClass_TYPE, sipTransferObj);
			if (!pobj) {
				// There was an error so garbage collect the Python list.
				Py_DECREF(l);
				return NULL;
			}
			PyList_SET_ITEM(PyList_GET_ITEM(l, i), j, pobj);
		}

		// Return the Python list.
		return l;
	%End

	%ConvertToTypeCode
		// Check if type is compatible
		if(!sipIsErr) 
		{
			if(!PyList_Check(sipPy))
				return 0;
				
			for(size_t i=0;i<PyObject_Size(sipPy);i++)
				if(!PyList_Check(PyList_GetItem(sipPy, i)))
					return 0;
			
			return 1;
		}
		
		auto result=new std::vector<std::vector<TYPE>>();
		
		for(size_t i=0;i<PyObject_Size(sipPy);i++)
		{
			result->push_back(std::vector<TYPE>());
			auto li=PyList_GetItem(sipPy, i);
			
			for(size_t j=0;j<PyObject_Size(li);j++)
			{
				auto item=PyList_GetItem(li, j);
				
				if(!sipCanConvertToInstance(item, sipClass_TYPE, SIP_NOT_NONE)) 
				{
					PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to TYPE");
					*sipIsErr=1;
					break;
				}
				
				int state;
				TYPE* p=reinterpret_cast<TYPE*>(
					sipConvertToInstance(item, sipClass_TYPE, 0, SIP_NOT_NONE, &state, sipIsErr));
				
				if(!*sipIsErr)
					(*result)[i].push_back(*p);
				
				sipReleaseInstance(p, sipClass_TYPE, state);
				Py_DECREF(item);
			}
		}
		
		if(*sipIsErr)
			return 0;
		
		*sipCppPtr=result;
		return sipGetState(sipTransferObj);
		
	%End
};
*/

// ****************************************************
// Specialization for std::vector<double>
// ****************************************************

%MappedType std::vector<double>
{
	%TypeHeaderCode
	#include <vector>
	%End

	%ConvertFromTypeCode
		PyObject *l;

		// Create the Python list of the correct length.
		if ((l = PyList_New(sipCpp -> size())) == NULL)
			return NULL;

		// Go through each element in the C++ instance and convert it to a
		// wrapped object.
		for (int i = 0; i < (int)sipCpp -> size(); ++i)
		{
			// Add the wrapper to the list.
			PyList_SET_ITEM(l, i, PyFloat_FromDouble(sipCpp -> at(i)));
		}

		// Return the Python list.
		return l;
	%End

	%ConvertToTypeCode
		// Check if type is compatible
		if (sipIsErr == NULL)
		{
			// Must be any iterable
			PyObject *i = PyObject_GetIter(sipPy);
			bool iterable = (i != NULL);
			Py_XDECREF(i);
			return iterable;
		}

		// Iterate over the object
		PyObject *iterator = PyObject_GetIter(sipPy);
		PyObject *item;

		// Maximum number of elements
		int len = PyObject_Size(sipPy);
		std::vector<double> *V = new std::vector<double>();
		V->reserve(len);

		if (len)
		{
			while ((item = PyIter_Next(iterator)))
			{
				if (!PyNumber_Check(item))
				{
					PyErr_Format(PyExc_TypeError, "object in iterable is not a number");
					*sipIsErr = 1;
					break;
				}

				PyObject *f = PyNumber_Float(item);
				V->push_back(PyFloat_AsDouble(f));

				Py_DECREF(f);
				Py_DECREF(item);
			}

			Py_DECREF(iterator);

			if (*sipIsErr)
			{
				delete V;
				return 0;
			}
		}

		*sipCppPtr = V;
		return sipGetState(sipTransferObj);
	%End
};

// ****************************************************
// Specialization for std::vector<int>
// ****************************************************

%MappedType std::vector<int>
{
	%TypeHeaderCode
	#include <vector>
	%End

	%ConvertFromTypeCode
		PyObject *l;

		// Create the Python list of the correct length.
		if ((l = PyList_New((SIP_SSIZE_T)sipCpp -> size())) == NULL)
			return NULL;

		// Go through each element in the C++ instance and convert it to a
		// wrapped object.
		for (int i = 0; i < (int)sipCpp -> size(); ++i)
		{
			// Add the wrapper to the list.
			PyList_SET_ITEM(l, i, PyLong_FromLong(sipCpp -> at(i)));
		}

		// Return the Python list.
		return l;
	%End

	%ConvertToTypeCode
		// Check if type is compatible
		if (sipIsErr == NULL)
		{
			// Must be any iterable
			PyObject *i = PyObject_GetIter(sipPy);
			bool iterable = (i != NULL);
			Py_XDECREF(i);
			return iterable;
		}

		// Iterate over the object
		PyObject *iterator = PyObject_GetIter(sipPy);
		PyObject *item;

		// Maximum number of elements
		int len = PyObject_Size(sipPy);
		std::vector<int> *V = new std::vector<int>();
		V->reserve(len);

		if (len)
		{
			while ((item = PyIter_Next(iterator)))
			{
				if (!PyLong_Check(item))
				{
					PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to float");
					*sipIsErr = 1;
					break;
				}

				int val = (int)PyLong_AsLong(item);
				V->push_back(val);

				Py_DECREF(item);
			}

			Py_DECREF(iterator);

			if (*sipIsErr)
			{
				delete V;
				return 0;
			}
		}

		*sipCppPtr = V;
		return sipGetState(sipTransferObj);
	%End
};
