////////////////////////////////////////////////////////////////////////////
//
// nrCmdLine.h
//
// A class for parsing command line switches.
//
// Nate Robins, January 2002.
//
// Example usage:
//
//    // Enumerate the command line arguments.
//    nrCmdLineArg args[] = 
//    {
//        nrCmdLineArg( 0,          "<scene_file>",             0, "file containing scene description",  g_Options.scene,  sizeof ( g_Options.scene ) ),
//        nrCmdLineArg( "-o",       "<output_file>", "output.tga", "output image filename",              g_Options.output, sizeof ( g_Options.output ) ),
//        nrCmdLineArg( "-w",       "<width>",              "256", "width of output image",              g_Options.width ),
//        nrCmdLineArg( "-h",       "<height>",             "256", "height of output image",             g_Options.height ),
//        nrCmdLineArg( "-shadows", "<true/false>",        "true", "generate shadow rays",               g_Options.shadows ),
//        nrCmdLineArg( "-bvh",     "<true/false>",        "true", "generate bounding volume hierarchy", g_Options.bvh ),
//        nrCmdLineArg( "-cull",    "<true/false>",       "false", "cull backfacing triangles",          g_Options.cull ),
//    };
//    
//    // Parse the command line.
//    nrCmdLine cmdline( args, sizeof ( args ) / sizeof ( nrCmdLineArg ) );
//    if ( ! cmdline.Parse( argc, argv ) )
//    {
//        cmdline.Usage( argv[ 0 ] );
//        return 1;
//    }
//
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRCMDLINE_H
#define NRCMDLINE_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrCmdLineArg
{
    friend class nrCmdLine;
    
public:
    
    nrCmdLineArg( const char* _switch, const char* parameter, const char* _default, const char* description, int& variable );
    nrCmdLineArg( const char* _switch, const char* parameter, const char* _default, const char* description, bool& variable );
    nrCmdLineArg( const char* _switch, const char* parameter, const char* _default, const char* description, float& variable );
    nrCmdLineArg( const char* _switch, const char* parameter, const char* _default, const char* description, char* variable, int length );
    
private:

    void Parse( const char* parameter );
    
private:
    
    enum
    {
        CMDLINEARG_INT,
        CMDLINEARG_BOOL,
        CMDLINEARG_FLOAT,
        CMDLINEARG_STRING,
    };
    
private:
    
    const char* m_Switch;
    const char* m_Parameter;
    const char* m_Default;
    const char* m_Description;
    int         m_Type;
    int         m_Length;
    bool        m_Parsed;
    
    union
    {
        int*   m_Int;
        bool*  m_Bool;
        float* m_Float;
        char*  m_String;
    };
};

////////////////////////////////////////////////////////////////////////////

class nrCmdLine
{
public:
    
    nrCmdLine( nrCmdLineArg* switches, int num_switches );
    ~nrCmdLine();
    
public:
    
    // Parse a command line.
    //
    // Returns true if parsed successfully, false otherwise.
    bool Parse( int argc, const char** argv );
    
    // Generate a usage message.
    void Usage( const char* program_name ) const;
    
private:
    
    int           m_NumArgs;
    nrCmdLineArg* m_Args;
};


////////////////////////////////////////////////////////////////////////////

#endif  // NRCMDLINE_H
