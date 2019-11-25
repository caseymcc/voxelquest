#ifndef _voxelquest_wsrequesthandler_h_
#define _voxelquest_wsrequesthandler_h_


class WebSocketRequestHandler: public HTTPRequestHandler
{
public:
    WebSocketRequestHandler(WebSocketServer* _ws_ptr);
    ~WebSocketRequestHandler();

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

private:

    WebSocketServer* ws_ptr;

};

#endif//_voxelquest__h_