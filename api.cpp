#include "api.hpp"

namespace Kavenegar {

  using namespace rapidjson;

  Api::Api()
  {

  }

  Api::~Api()
  {

  }

  std::string Api::whitespace_reduce(std::string str)
  {
    uint64_t index; //!ToDo Fixing for x86 Platform...
    while ((index = str.find("  ")) != std::string::npos)
      str.erase(index, 1);
    return str;
  }

  std::string Api::whitespace_leading(std::string str)
  {
    while (std::isspace(*str.begin()))
      str.erase(str.begin());
    return str;
  }

  std::string Api::escapeJSON(const std::string& input)
  {
    std::string output;
    output.reserve(input.length());

    for (std::string::size_type i = 0; i < input.length(); ++i)
      {
        switch (input[i]) {
          case '"':
            output += "\\\"";
            break;
          case '/':
            output += "\\/";
            break;
          case '\b':
            output += "\\b";
            break;
          case '\f':
            output += "\\f";
            break;
          case '\n':
            output += "\\n";
            break;
          case '\r':
            output += "\\r";
            break;
          case '\t':
            output += "\\t";
            break;
          case '\\':
            output += "\\\\";
            break;
          default:
            output += input[i];
            break;
          }

      }

    return whitespace_reduce(output);
  }

  std::string Api::unescapeJSON(const std::string& input)
  {
    State s = UNESCAPED;
    std::string output;
    output.reserve(input.length());

    for (std::string::size_type i = 0; i < input.length(); ++i)
      {
        switch(s)
          {
          case ESCAPED:
            {
              switch(input[i])
                {
                case '"':
                  output += '\"';
                  break;
                case '/':
                  output += '/';
                  break;
                case 'b':
                  output += '\b';
                  break;
                case 'f':
                  output += '\f';
                  break;
                case 'n':
                  output += '\n';
                  break;
                case 'r':
                  output += '\r';
                  break;
                case 't':
                  output += '\t';
                  break;
                case '\\':
                  output += '\\';
                  break;
                default:
                  output += input[i];
                  break;
                }

              s = UNESCAPED;
              break;
            }
          case UNESCAPED:
            {
              switch(input[i])
                {
                case '\\':
                  s = ESCAPED;
                  break;
                default:
                  output += input[i];
                  break;
                }
            }
          }
      }
    return output;
  }


  ApiException::ApiException()
  {

  }

  ApiException::~ApiException()
  {

  }

  void ApiException::setMessage(const std::string &message)
  {
    m_message = message;
  }

  void ApiException::setCode(const int &code)
  {
    m_code = code;
  }

  std::string ApiException::getMessage()
  {
    std::string temp;

    Document document;

    std::cout << unescapeJSON(m_message.c_str()) << std::endl;

    document.Parse(unescapeJSON(m_message.c_str()).c_str());

    temp = document["return"]["message"].GetString();

    return temp;
  }

  int ApiException::getCode()
  {
    int statusCode;

    Document document;

    std::cout << unescapeJSON(m_message.c_str()) << std::endl;

    document.Parse(unescapeJSON(m_message.c_str()).c_str());

    statusCode = document["return"]["status"].GetInt();

    switch (statusCode) {
      case ReturnCode::Ret200:
        statusCode = 200;
        break;
      case ReturnCode::Ret400:
        statusCode = 400;
        break;
      case ReturnCode::Ret401:
        statusCode = 401;
        break;
      case ReturnCode::Ret402:
        statusCode = 402;
        break;
      case ReturnCode::Ret403:
        statusCode = 403;
        break;
      case ReturnCode::Ret404:
        statusCode = 404;
        break;
      case ReturnCode::Ret405:
        statusCode = 405;
        break;
      case ReturnCode::Ret406:
        statusCode = 406;
        break;
      case ReturnCode::Ret407:
        statusCode = 407;
        break;
      case ReturnCode::Ret409:
        statusCode = 409;
        break;
      case ReturnCode::Ret411:
        statusCode = 411;
        break;
      case ReturnCode::Ret412:
        statusCode = 412;
        break;
      case ReturnCode::Ret413:
        statusCode = 414;
        break;
      case ReturnCode::Ret414:
        statusCode = 414;
        break;
      case ReturnCode::Ret415:
        statusCode = 415;
        break;
      case ReturnCode::Ret416:
        statusCode = 416;
        break;
      case ReturnCode::Ret417:
        statusCode = 417;
        break;
      case ReturnCode::Ret418:
        statusCode = 418;
        break;
      case ReturnCode::Ret419:
        statusCode = 419;
        break;
      case ReturnCode::Ret422:
        statusCode = 422;
        break;
      case ReturnCode::Ret424:
        statusCode = 424;
        break;
      case ReturnCode::Ret426:
        statusCode = 426;
        break;
      case ReturnCode::Ret428:
        statusCode = 428;
        break;
      case ReturnCode::Ret431:
        statusCode = 431;
        break;
      case ReturnCode::Ret432:
        statusCode = 432;
        break;
      default:
        break;
      }

    return statusCode;
  }

}
