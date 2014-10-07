#pragma once

// TString.h
//
// Fornece uma abstração de string para o tipo tchar.h.
// Inclui o C++ std::string e as classes do IO Stream associadas.
//
// Para mais detalhes sobre as classes string, iostream e stringstream da 
// Standard Template Library, veja o livro "C++ Standard Library", do 
// Nicolay Josuttis: http://astore.amazon.com/pon098-20/detail/0201379260
//
// Esse arquivo estende as idéias em tchar.h para as classes e streams da STL. 
//
// Fornece:
//  mage::TString
//  mage::TIStringstream, mage::TOStringstream, mage::tstringstream
//  mage::TIStream,       mage::TOStream,       mage::tiostream
//  mage::TIFStream,      mage::TOFstream,      mage::tfstream
//
//  tcin, tcout, tcerr, tclog
//

#include <tchar.h>

#if defined(UNICODE) || defined(_UNICODE)
#define RT_UNICODE
#else
#undef RT_UNICODE
#endif

// Declarações para as classes da STL
#include <iosfwd>
#include <string>

namespace mage {
    // TCHAR string
    typedef std::basic_string<TCHAR> TString;

    // TCHAR string streams
    typedef std::basic_istringstream<TCHAR> TIStringStream;
    typedef std::basic_ostringstream<TCHAR> TOStringstream;
    typedef std::basic_stringstream<TCHAR> TStringStream;

    // TCHAR iostreams
    typedef std::basic_istream<TCHAR> TIStream;
    typedef std::basic_ostream<TCHAR> TOStream;
    typedef std::basic_iostream<TCHAR> TIOStream;

    // TCHAR file io streams; use com cuidado - normalmente queremos arquivos ansii
    typedef std::basic_ifstream<TCHAR> TIFStream;
    typedef std::basic_ofstream<TCHAR> TOFStream;
    typedef std::basic_fstream<TCHAR> TFStream;
};

// streams padrão
#if defined(RT_UNICODE)

#define tcin  wcin
#define tcout wcout
#define tcerr wcerr
#define tclog wclog

#else

#define tcin  cin
#define tcout cout
#define tcerr cerr
#define tclog clog

#endif