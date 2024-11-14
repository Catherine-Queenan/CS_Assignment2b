//
// Created by eugen on 2024-11-08.
//

#include "UploadServlet.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/dir.h>

#include <filesystem>

namespace fs = std::filesystem;

map<string,string> UploadServlet::getFormData(const string& boundary, const string& input) {
    //Used for finding form data
    const string CAPTION_DELIM = "Content-Disposition: form-data; name=\"caption\"";
    const string DATE_DELIM = "Content-Disposition: form-data; name=\"date\"";
    const string FILENAME_DELIM = "Content-Disposition: form-data; name=\"File\"; filename=\"";
    const string END_BOUNDARY = boundary + "--";

    //For parsing request
    stringstream in{input};

    //Store data
    string requestLine;
    map<string, string> formData;

    //Parse request and find each piece of form data
    while(getline(in, requestLine, '\r')) {

        if(requestLine.find(CAPTION_DELIM) != string::npos) {
            getline(in, requestLine, '\r');
            getline(in, requestLine, '\r');

            formData["caption"] = requestLine.substr(1);
        } else if(requestLine.find(DATE_DELIM) != string::npos) {
            getline(in, requestLine, '\r');
            getline(in, requestLine, '\r');

            formData["date"] = requestLine.substr(1);
        } else if(requestLine.find(FILENAME_DELIM) != string::npos) {
            string fileNameDelim = "filename=";
            string fileName = requestLine.substr((requestLine.find(fileNameDelim) + fileNameDelim.size() + 1));
            fileName.pop_back();

            formData["fileName"] = fileName;

            getline(in, requestLine, '\r');
            getline(in, requestLine, '\r');

            //Get file bytes
            string file;
            while(getline(in, requestLine)) {
                file.append((requestLine + "\n"));

                if(requestLine.find(END_BOUNDARY) != string::npos) {
                    file = file.substr(1, file.find(END_BOUNDARY));
                    break;
                }
            }
            formData["file"] = file;
        }

        if(requestLine.find(END_BOUNDARY) != string::npos) {
            break;
        }
    }

    return formData;
}

string UploadServlet::getFilesList() {
    string htmlResponse;
    struct direct *d;

    DIR *dirp = opendir("../uploads");
    if (dirp == nullptr) {
        perror("opening dir");
    }

    while ((d = readdir(dirp))) {
        htmlResponse.append("<li>").append(d -> d_name).append("</li>");
    }
    closedir (dirp);

    return htmlResponse;
}

void UploadServlet::doGet(HttpServletRequest& req, HttpServletResponse& res) {
    try {
        res.setContentType("text/html");
        string response;
        response.append("<html><body>\n");
        response.append("<h1>File Upload Form</h1>\n");
        response.append("<form method='POST' enctype='multipart/form-data' action=\"http://localhost:8083\">\n");
        response.append("Caption: <input type=\"text\" name=\"caption\"/><br><br>\n");
        response.append("Date: <input type=\"date\" name=\"date\" /><br><br>\n");
        response.append("<input type=\"file\" name=\"File\"/><br><br>\n");
        response.append("<input type=\"submit\" value=\"Submit\" />\n");
        response.append("</form>\n");
        response.append("</body></html>\n");
        res.sendResponse(response);
    } catch(...){}

}

void UploadServlet::doPost(HttpServletRequest& req, HttpServletResponse& res) {
    const string DIRECTORY = "../uploads/";

    const string in = req.getInputStreamString();
    const string boundary = "--" + req.getBoundary();

    map<string, string> formData = getFormData(boundary, in);
    string fileName = formData["caption"] + "_" + formData["date"] + "_" + formData["fileName"];

    string file = formData["file"];

    if(file.empty()) {
        res.setContentType("text/html");
        string htmlResponse = "<!Doctype html><html><head><title>Error</title></head><body><p>An issue occurred during "
                              "file upload</p></body></html>";
        res.sendResponse(htmlResponse);
        return;
    }

    ofstream outputFile;
    outputFile.open((DIRECTORY+ fileName), std::ios::out | std::ios::binary);

    if (!outputFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    if (outputFile.is_open()) {
        vector<char> binaryContent(file.begin(), file.end());
        outputFile.write(binaryContent.data(), binaryContent.size());
        outputFile.close();

        std::cout << "File saved: " << DIRECTORY + fileName << std::endl;
    } else {
        std::cerr << "Failed to save file: " << DIRECTORY + fileName << std::endl;
    }

    res.setContentType("text/html");
    string htmlResponse = "<!Doctype html><html><head><title>Uploaded Files</title></head><body><ul>";
    htmlResponse.append(getFilesList()).append("</ul></body></html>");

    res.sendResponse(htmlResponse);
}




