#include "network.h"

void WebSocketRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{

    JSONValue *value;



    Application& app=Application::instance();



    try
    {


        WebSocket ws(request, response);
        app.logger().information("WebSocket connection established.");
        //char buffer[1024];
        int flags;
        int n;


        do
        {
            n=ws.receiveFrame(g_RecBuffer, g_MAX_FRAME_SIZE, flags);
            app.logger().information(Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)));
            ws.sendFrame(g_RecBuffer, n, flags);


            if(n>0)
            {

                g_RecBuffer[n+1]='\0';


                value=JSON::Parse(g_RecBuffer);
                if(value==NULL)
                {
                    app.logger().information("Invalid JSON\n\n");
                }
                else
                {
                    app.logger().information("Valid JSON\n\n");

                    doTrace("JSON VAL", f__s(value->Child(L"x")->number_value), "\n\n");

                    delete value;
                    value=NULL;

                }

            }

        } while((n>0||(flags & WebSocket::FRAME_OP_BITMASK)!=WebSocket::FRAME_OP_CLOSE)&&PROG_ACTIVE);
        app.logger().information("WebSocket connection closed.");






    }
    catch(WebSocketException& exc)
    {
        app.logger().log(exc);
        switch(exc.code())
        {
        case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
            response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            // fallthrough
        case WebSocket::WS_ERR_NO_HANDSHAKE:
        case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
        case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
            response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
            response.setContentLength(0);
            response.send();
            break;
        }
    }






}


HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    Application& app=Application::instance();
    app.logger().information("Request from "
        +request.clientAddress().toString()
        +": "
        +request.getMethod()
        +" "
        +request.getURI()
        +" "
        +request.getVersion());

    for(HTTPServerRequest::ConstIterator it=request.begin(); it!=request.end(); ++it)
    {
        app.logger().information(it->first+": "+it->second);
    }

    return new WebSocketRequestHandler;
}


void WebSocketServer::initialize(Application& self)
{
    loadConfiguration(); // load default configuration files, if present
    ServerApplication::initialize(self);
}

void WebSocketServer::uninitialize()
{
    ServerApplication::uninitialize();
}

void WebSocketServer::defineOptions(OptionSet& options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
        Option("help", "h", "display help information on command line arguments")
        .required(false)
        .repeatable(false));
}

void WebSocketServer::handleOption(const std::string& name, const std::string& value)
{
    ServerApplication::handleOption(name, value);

    if(name=="help")
        _helpRequested=true;
}

void WebSocketServer::displayHelp()
{
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("A sample HTTP server supporting the WebSocket protocol.");
    helpFormatter.format(std::cout);
}


int WebSocketServer::main(const std::vector<std::string>& args)
{

    g_RecBuffer=new char[g_MAX_FRAME_SIZE];

    // get parameters from configuration file
    unsigned short port=(unsigned short)config().getInt("WebSocketServer.port", 9980);

    // set-up a server socket
    ServerSocket svs(port);
    // set-up a HTTPServer instance
    HTTPServer srv(new RequestHandlerFactory, svs, new HTTPServerParams);
    // start the HTTPServer
    srv.start();
    // wait for CTRL-C or kill
    //waitForTerminationRequest();
    // Stop the HTTPServer

    glutMainLoop();

    srv.stop();

    delete[] g_RecBuffer;

    return Application::EXIT_OK;

    /*
    if (_helpRequested)
    {
        displayHelp();
    }
    else
    {
        // get parameters from configuration file
        unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 9980);

        // set-up a server socket
        ServerSocket svs(port);
        // set-up a HTTPServer instance
        HTTPServer srv(new RequestHandlerFactory, svs, new HTTPServerParams);
        // start the HTTPServer
        srv.start();
        // wait for CTRL-C or kill
        waitForTerminationRequest();
        // Stop the HTTPServer
        srv.stop();
    }
    return Application::EXIT_OK;

    return 0;
    */
}


