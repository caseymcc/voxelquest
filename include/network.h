#ifndef _voxelquest_networl_h_
#define _voxelquest_networl_h_

char* g_RecBuffer;
static int g_MAX_FRAME_SIZE=16777216; //16 MB

class WebSocketRequestHandler: public HTTPRequestHandler
    /// Handle a WebSocket connection.
{
public:

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

};


class RequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

};


class WebSocketServer: public Poco::Util::ServerApplication
    /// The main application class.
    ///
    /// This class handles command-line arguments and
    /// configuration files.
    /// Start the WebSocketServer executable with the help
    /// option (/help on Windows, --help on Unix) for
    /// the available command line options.
    ///
    /// To use the sample configuration file (WebSocketServer.properties),
    /// copy the file to the directory where the WebSocketServer executable
    /// resides. If you start the debug version of the WebSocketServer
    /// (WebSocketServerd[.exe]), you must also create a copy of the configuration
    /// file named WebSocketServerd.properties. In the configuration file, you
    /// can specify the port on which the server is listening (default
    /// 9980) and the format of the date/time string sent back to the client.
    ///
    /// To test the WebSocketServer you can use any web browser (http://localhost:9980/).
{
public:
    WebSocketServer(): _helpRequested(false)
    {
        dataReady=false;
    }

    ~WebSocketServer()
    {}

    //int main(const std::vector<std::string>& args);

protected:
    void initialize(Application& self);


    void uninitialize();


    void defineOptions(OptionSet& options);


    void handleOption(const std::string& name, const std::string& value);


    void displayHelp();



    int main(const std::vector<std::string>& args);



private:
    bool dataReady;
    bool _helpRequested;
};





#endif//_voxelquest__h_