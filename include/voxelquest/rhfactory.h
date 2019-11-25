#ifndef _voxelquest_rhfactory_h_
#define _voxelquest_rhfactory_h_


class RequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
    RequestHandlerFactory(WebSocketServer* _ws_ptr);

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

private:
    WebSocketServer* ws_ptr;

};



#endif//_voxelquest__h_