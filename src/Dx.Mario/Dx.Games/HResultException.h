#pragma once
#include <d3d9.h>
#include <DxErr.h>
#include <mage/TString.h>
#include <string>

namespace games {
    class HResultException
    {
    public:

        HResultException(const std::string& message, HRESULT hr) :
            hresult(hr), message(message), error(DXGetErrorString(hr)), description(DXGetErrorDescription(hr))
        {
        }

        ~HResultException()
        {
        }

        HRESULT hresult;
        std::string message;
        mage::TString error;
        mage::TString description;

        const char* what() const throw() { return message.c_str(); }
    };
}
