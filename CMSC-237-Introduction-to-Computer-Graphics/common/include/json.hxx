/*! \file json.hxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _JSON_HXX_
#define _JSON_HXX_

#include <vector>
#include <string>
#include <map>

namespace JSON {

  //! the types of JSON values
    enum Type {
	T_OBJECT,	//!< object consisting of name-value pairs
	T_ARRAY,	//!< arrays of JSON values
	T_NUMBER,	//!< numbers (represented as doubles)
	T_STRING,	//!< strings
	T_BOOL,		//!< booleans
	T_NULL		//!< the null value
    };

    class Value;
    class Object;
    class Array;
    class Number;
    class String;
    class Bool;
    class Null;

  // parse a JSON file; this returns nullptr if there is a parsing error
    Value *ParseFile (std::string filename);

  // virtual base class of JSON values
    class Value {
      public:

      //! return the type of this JSON value
	Type type() const { return this->_ty; }

      //! return true if this value is an object
	bool isObject() const { return (this->_ty == T_OBJECT); }

      //! return true if this value is an array
	bool isArray() const { return (this->_ty == T_ARRAY); }

      //! return true if this value is a number
	bool isNumber() const { return (this->_ty == T_NUMBER); }

      //! return true if this value is a string
	bool isString() const { return (this->_ty == T_STRING); }

      //! return true if this value is a boolean
	bool isBool() const { return (this->_ty == T_BOOL); }

      //! return true if this value is the null value
	bool isNull() const { return (this->_ty == T_NULL); }

      //! dynamic cast to JSON object
	const Object *asObject () const;

      //! dynamic cast to JSON object
	const Array *asArray () const;

      //! dynamic cast to JSON object
	const Number *asNumber () const;

      //! dynamic cast to JSON object
	const String *asString () const;

      //! dynamic cast to JSON object
	const Bool *asBool () const;

	virtual ~Value() = 0;
	virtual std::string toString() = 0;

      protected:

	explicit Value (Type ty) : _ty(ty) { };

	Type	_ty;
    };

    inline std::ostream& operator<< (std::ostream& s, Value *v)
    {
	return s << v->toString();
    }

  //! JSON objects
    class Object : public Value {
      public:
	Object ();
	~Object ();

      //! return the number of fields in the object
	int size () const { return this->_value.size(); }

      //! insert a key-value pair into the object
	void insert (std::string key, Value *val);

      //! return the value corresponding to the given key.
      //! \returns nil if the key is not defined in the object
	Value *operator[] (std::string key) const;

	std::string toString();

      private:
	std::map<std::string, Value *> _value;
    };

  //! JSON arrays
    class Array : public Value {
      public:
	Array ();
	~Array ();

	int length () const { return static_cast<int>(this->_value.size()); }
	void add (Value *v) { this->_value.push_back(v); }

	Value *operator[] (int idx) const { return this->_value[idx]; }

	std::string toString();

      private:
	std::vector<Value *> _value;
    };

    class Number : public Value {
      public:
	Number (double v);
	~Number ();

	double value () const { return this->_value; }

	std::string toString();

      private:
	double _value;
    };

    class String : public Value {
      public:
	String (std::string v);
	~String ();

	std::string value () const { return this->_value; }

	std::string toString();

      private:
	std::string _value;
    };

    class Bool : public Value {
      public:
	Bool (bool v);
	~Bool ();

	bool value () const { return this->_value; }

	std::string toString();

      private:
	bool _value;
    };

    class Null : public Value {
      public:
	Null ();
	~Null ();

	std::string toString();

    };

} // namespace JSON

#endif // !_JSON_HXX_
