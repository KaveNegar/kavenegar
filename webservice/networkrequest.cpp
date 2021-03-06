﻿#include "networkrequest.hpp"
#include "api.hpp"
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace WebService {

  NetworkRequest::NetworkRequest()
  {
    this->result = "Unknown";

    curl = curl_easy_init();
    if(!curl)
      throw std::string ("Curl did not initialize!");

  }

  NetworkRequest::~NetworkRequest()
  {
    cleanGlobal();
  }

  static size_t WriteCallback2(void *contents, size_t size, size_t nmemb, void *userp)
  {
      ((std::string*)userp)->append((char*)contents, size * nmemb);
      return size * nmemb;
  }

  size_t NetworkRequest::WriteCallback(char *data, size_t size, size_t nmemb, std::string *buffer)
  {
    unsigned long result = 0;
    if (buffer != nullptr) {
        buffer->append(data, size * nmemb);
        *buffer += data;
        result = size * nmemb;
      }
    return result;
  }

  void NetworkRequest::clean() {
    curl_easy_cleanup(curl);
  }

  void NetworkRequest::cleanGlobal() {
    curl_global_cleanup();
  }

  void NetworkRequest::setContentType(Headers c){

    switch (c) {
      case Headers::X_Urlencoded:
        headerType = ContentTypeHeader::APPLICATION_X_WWW_FROM_URLENCODED;
        break;
      case Headers::JSon:
        headerType = ContentTypeHeader::APPLICATION_JSON;
        break;
      case Headers::XHtml:
        headerType = ContentTypeHeader::APPLICATION_XHTML;
        break;
      case Headers::Xml:
        headerType = ContentTypeHeader::APPLICATION_XML;
        break;
      case Headers::Html:
        headerType = ContentTypeHeader::TEXT_HTML;
        break;
      case Headers::MultiPart:
        headerType = ContentTypeHeader::MULTIPART_FROM_DATA;
        break;
      }

  }

  void NetworkRequest::post(const std::string &url, const std::string &query){

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    //curl = curl_easy_init();

    if(curl) {

        ctype_list = curl_slist_append(ctype_list, headerType.c_str());


        if(isset(headerType)) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, ctype_list);
            std::clog << "Content type has been set!" << std::endl;
          }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);



        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        curl_slist_free_all(ctype_list); /* free the ctype_list again */

        setResult(readBuffer);

        /* Check for errors */
        if(res != CURLE_OK)
          std::clog << curl_easy_strerror(res) << std::endl;

        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        clean();
      }
  }

  void NetworkRequest::get(const std::string &url)
  {

    std::string readBuffer2;

    if(curl) {
//        if(isset(headerType)) {
//            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, ctype_list);
//            std::clog << "Content type has been set!" << std::endl;
//          }

#ifdef SKIP_PEER_VERIFICATION
        /*
       * If you want to connect to a site who isn't using a certificate that is
       * signed by one of the certs in the CA bundle you have, you can skip the
       * verification of the server's certificate. This makes the connection
       * A LOT LESS SECURE.
       *
       * If you have a CA cert for the server stored someplace else than in the
       * default bundle, then the CURLOPT_CAPATH option might come handy for
       * you.
       */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
        /*
       * If the site you're connecting to uses a different host name that what
       * they have mentioned in their server certificate's commonName (or
       * subjectAltName) fields, libcurl will refuse to connect. You can skip
       * this check, but this will make the connection less secure.
       */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        //curl_easy_setopt(curl, CURLOPT_HEADER, false);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback2);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer2);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

        /* always cleanup */
        //clean();
        curl_easy_cleanup(curl);


        std::clog << "Original : " << readBuffer2 << std::endl;
        setResult(readBuffer2);

      }
  }


  void NetworkRequest::setResult(const std::string &res) {

    Kavenegar::ApiException apiEx;

    if(!res.empty()) {
        result = res;
        apiEx.setMessage(result);
      } else {
        result = "unknown";
        apiEx.setMessage("Unknown");
      }
  }

  std::string NetworkRequest::getResult() {
    return result;
  }


}
