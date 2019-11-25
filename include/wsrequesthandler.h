

class WebSocketRequestHandler: public HTTPRequestHandler
{
public:
    WebSocketRequestHandler(WebSocketServer* _ws_ptr);
    ~WebSocketRequestHandler();

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

private:

    WebSocketServer* ws_ptr;

};
