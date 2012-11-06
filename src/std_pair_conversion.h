#ifndef BOOST_ADAPTBX_STD_PAIR_CONVERSION_H
#define BOOST_ADAPTBX_STD_PAIR_CONVERSION_H

#include <boost/python/tuple.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/to_python_converter.hpp>

namespace boost_adaptbx { namespace std_pair_conversions {

  namespace detail {
    template <typename T, typename U>
    struct to_tuple
    {
      static PyObject* convert(std::pair<T,U> const& p) {
        using namespace boost::python;
        return incref(boost::python::make_tuple(p.first, p.second).ptr());
      }

      static PyTypeObject const *get_pytype() { return &PyTuple_Type; }
    };
  }

  template <typename T, typename U>
  struct to_tuple
  {
    to_tuple() {
      using namespace boost::python;
      to_python_converter<std::pair<T,U>, detail::to_tuple<T,U>
#ifdef BOOST_PYTHON_SUPPORTS_PY_SIGNATURES
                                    , true
#endif
      >();
    }
  };

  using namespace boost::python;

  template <typename T, typename U>
  struct from_tuple
  {
    typedef std::pair<T,U> pair_t;

    from_tuple() {
      using namespace boost::python::converter;
      registry::push_back(&convertible,
          &construct,
          boost::python::type_id<pair_t>()
          );
    }

    static void* convertible(PyObject* py_obj)
    {
      if (convertible_impl(py_obj, 0)) {
        return py_obj;
      } else {
        return 0;
      }
    }

    static void construct(PyObject* py_obj, converter::rvalue_from_python_stage1_data* data)
    {
      typedef converter::rvalue_from_python_storage<pair_t> storage_t;
      storage_t* the_storage = reinterpret_cast<storage_t*>(data);
      void* memory_chunk = the_storage->storage.bytes;
      pair_t* cpp_obj = new (memory_chunk) pair_t();
      data->convertible = memory_chunk;

      if (!convertible_impl(py_obj, cpp_obj)) {
        throw std::runtime_error("Object not convertible");
      }
    }

    private:
    static bool convertible_impl(PyObject *py_obj, pair_t *t) 
    {
      if (!PySequence_Check(py_obj)) {
        return false;
      }

      if (!PyObject_HasAttrString(py_obj, "__len__" )) {
        return false;
      }

      object py_sequence(handle<>(borrowed(py_obj)));

      if (len(py_sequence) != 2) {
        return false;
      }

      object element1 = py_sequence[0];
      object element2 = py_sequence[1];

      extract<T> type_checker1(element1);
      extract<U> type_checker2(element2);

      if (t == 0) {
        if (!type_checker1.check() || !type_checker2.check()) {
          return false;
        }
      } else {
        t->first = type_checker1();
        t->second = type_checker2();
      }
      return true;
    }
  };

  template <typename T, typename U>
  struct to_and_from_tuple
  {
    to_and_from_tuple() {
      to_tuple<T,U>();
      from_tuple<T,U>();
    }
  };


}}

#endif
