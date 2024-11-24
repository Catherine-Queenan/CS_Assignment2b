//
// Created by eugen on 2024-11-08.
//

#pragma once
#include "HttpServletRequest.h"
#include "HttpServletResponse.h"

// HttpServlet interface for use in HttpServletRequest and HttpServletResponse
// Defines our custom servlet doGet and doPost methods
class HttpServlet {
public:
    // Virtual destructor for the HttpServlet
    virtual ~HttpServlet() = default;

protected:
    // Pure virtual doGet method to query information from the server
    // PRE: request is an HttpServletRequest object
    // PRE: response is an HttpServletResponse object
    virtual void doGet(HttpServletRequest& req, HttpServletResponse& res)=0;

    // Pure virtual doPost method to update information to the server
    // PRE: request is an HttpServletRequest object
    // PRE: response is an HttpServletResponse object
    virtual void doPost(HttpServletRequest& req, HttpServletResponse& res)=0;
};
