//
// Created by eugen on 2024-11-08.
//

#pragma once
#include "HttpServletRequest.h"
#include "HttpServletResponse.h"


class HttpServlet {
public:
    virtual ~HttpServlet() = default;

protected:
    virtual void doGet(HttpServletRequest request, HttpServletResponse response)=0;
    virtual void doPost(HttpServletRequest request, HttpServletResponse response)=0;
};
