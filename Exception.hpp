#pragma once

#include <string>


namespace siilib {

class Exception : public std::exception {
protected:
    std::string msg;
public:
    Exception(std::string msg) : msg(msg) { }
    const char* what() const noexcept override { return msg.c_str(); }
};



class LookupException : public Exception {
public:
    LookupException(std::string msg) : Exception(msg) { }
};

class IndexError : public LookupException {
public:
    IndexError() : LookupException("Invalid element index") { }
};

class KeyError : public LookupException {
public:
    KeyError() : LookupException("Key not found") { }
};



class SizeException : public Exception {
public:
    SizeException(std::string msg) : Exception(msg) { }
};

class EmptyError : public SizeException {
public:
    EmptyError() : SizeException("Container is empty") { }
};

class OverflowError : public SizeException {
public:
    OverflowError() : SizeException("Container overflow") { }
};



class TypeError : public Exception {
public:
    TypeError() : Exception("Invalid type") { }
};


class ValueError : public Exception {
public:
    ValueError() : Exception("Invalid value") { }
};



class MemoryException : public Exception {
public:
    MemoryException(std::string msg) : Exception(msg) { }
};

class ResizeError : public MemoryException {
public:
    ResizeError() : MemoryException("Resize failed: bad_alloc") { }
};

class AllocError : public MemoryException {
public:
    AllocError() : MemoryException("Memory allocation failed: bad_alloc") { }
};


class ArithmeticException : public Exception {
public:
    ArithmeticException(std::string msg) : Exception(msg) { }
};
}
