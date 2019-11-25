

class RequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
    RequestHandlerFactory(WebSocketServer* _ws_ptr);

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

private:
    WebSocketServer* ws_ptr;

};


