#include <string>


namespace siilib {

class Exception : public std::exception {
protected:
    std::string msg;
public:
    Exception(std::string msg) : msg(msg) { }
    const char* what() const noexcept override { return msg.c_str(); }
};



class ArithmeticException : public Exception {
public:
    ArithmeticException(std::string msg) : Exception(msg) { }
};



class LookupException : public Exception {
public:
    LookupException(std::string msg) : Exception(msg) { }
};

class IndexError : public LookupException {
public:
    IndexError() : LookupException("Invalid element index") { }
};

class EmptyError : public LookupException {
public:
    EmptyError() : LookupException("Container is empty") { }
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
}
