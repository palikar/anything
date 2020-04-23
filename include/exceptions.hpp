#pragma once


#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::exc
{

struct engine_exception : public std::runtime_error
{
    engine_exception(std::string what) : runtime_error(format(what))
    {}

  protected:
    static std::string format(const std::string& what)
    {        
        return what;
    }    
};

struct rendering_exception : public std::runtime_error
{
    rendering_exception(std::string what) : runtime_error(format(what))
    {}

  protected:
    static std::string format(const std::string& what)
    {        
        return what;
    }    
};

struct math_exception : public std::runtime_error
{
    math_exception(std::string what) : runtime_error(format(what))
    {}

  protected:
    static std::string format(const std::string& what)
    {        
        return what;
    }    
};

struct graphics_exception : public std::runtime_error
{
    graphics_exception(std::string what) : runtime_error(format(what))
    {}

  protected:
    static std::string format(const std::string& what)
    {        
        return what;
    }    
};

struct application_exception : public std::runtime_error
{
    application_exception(std::string what) : runtime_error(format(what))
    {}

  protected:
    static std::string format(const std::string& what)
    {        
        return what;
    }    
};

}
