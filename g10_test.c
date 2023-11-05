/** !
 * Tester for g10
 * 
 * @file g10_test.c
 * 
 * @author Jacob Smith
 */

// Header
#include <sync/sync.h>
#include <log/log.h>
#include <g10/g10.h>

// Enumeration definitions
enum result_e {
    zero  = 0,
    one   = 1,
    match = 0,
};

// Type definitions
typedef enum result_e result_t;

// Global variables
int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

// Forward declarations
/** !
 * Print the time formatted in days, hours, minutes, seconds, miliseconds, microseconds
 * 
 * @param seconds the time in seconds
 * 
 * @return void
 */
void print_time_pretty ( double seconds );

/** !
 * Run all the tests
 * 
 * @param void
 * 
 * @return void
 */
void run_tests ( void );

/** !
 * Print a summary of the test scenario
 * 
 * @param void
 * 
 * @return void
 */
void print_final_summary ( void );

/** !
 * Print the result of a single test
 * 
 * @param scenario_name the name of the scenario
 * @param test_name     the name of the test
 * @param passed        true if test passes, false if test fails
 * 
 * @return void
 */
void print_test ( const char *scenario_name, const char *test_name, bool passed );

bool     test_parse_json     ( char         *test_file    , int         (*expected_value_constructor) (json_value **), result_t expected );
bool     test_serial_json    ( char         *test_file    , char         *expected_file                               , int(*expected_value_constructor) (json_value **), result_t expected );
result_t load_json           ( json_value  **pp_value     , char         *test_file );
result_t save_json           ( char         *path         , json_value   *p_value );
bool     value_equals        ( json_value   *a            , json_value   *b );
size_t   load_file           ( const char   *path         , void         *buffer                                      , bool     binary_mode );

bool test_g_init ( char *test_file, int(*expected_g_instance_constructor) (g_instance **), result_t expected );
void test_g10_g_init ( const char *name );

// Entry point
int main ( int argc, const char* argv[] )
{
    
    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Initialize the timer library
    timer_init();
    log_init(0, false);

    // Formatting
    log_info("|============|\n| G10 TESTER |\n|============|\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("\ng10 tests took ");
    print_time_pretty ( (double) ( t1 - t0 ) / (double) timer_seconds_divisor() );
    log_info(" to test\n");

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void print_time_pretty ( double seconds )
{

    // Initialized data
    double _seconds = seconds;
    size_t days = 0,
           hours = 0,
           minutes = 0,
           __seconds = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) log_info("%zu D, ", days);
    
    // Print hours
    if ( hours ) log_info("%zu h, ", hours);

    // Print minutes
    if ( minutes ) log_info("%zu m, ", minutes);

    // Print seconds
    if ( __seconds ) log_info("%zu s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds ) log_info("%zu ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds ) log_info("%zu us", microseconds);
    
    // Done
    return;
}

void run_tests ( void )
{

    // Initialized data
    timestamp g10_t0 = 0,
              g10_t1 = 0;

    ///////////////////
    // Test the core //
    ///////////////////

    // Start
    g10_t0 = timer_high_precision();

    // Test g_init
    test_g10_g_init("g10_core_g_init");

    // Stop
    g10_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 core took ");
    print_time_pretty ( (double)(g10_t1-g10_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Done
    return;
}

void test_g10_g_init ( const char *name )
{
    
    // Formatting
    log_info("Scenario: %s\n", name);

    // Test an empty file
    print_test(name, "null", test_g_init(0, (void *) 0, match));
    print_test(name, "empty", test_g_init("test cases/core/empty.json", (void *) 0, match));
    print_test(name, "empty_object", test_g_init("test cases/core/empty_object.json", (void *) 0, match));
    //print_test(name, "null", test_g_init("test cases/core/empty.json", (void *) 0, zero));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

bool test_g_init ( char *test_file, int(*expected_g_instance_constructor) (g_instance **), result_t expected )
{
    
    // Initialized data
    bool ret = true;
    size_t file_len = 0;
    result_t result = 0,
             value_eq = 0;
    char *file_buf = 0;
    g_instance *p_expected_instance = 0,
               *p_return_instance = 0;

    // Construct the expected json value
    if (expected_g_instance_constructor) expected_g_instance_constructor(&p_expected_instance);

    // Parse the json value
    result = g_init ( &p_return_instance, test_file );

    // Free the json value
    if ( result ) g_exit(&p_return_instance);
    
    // Match
    if ( p_expected_instance == p_return_instance ) result = match;

    // Success
    return (result == expected);
}

void print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    if ( passed )
        log_pass("%s %s %s\n","[PASS]", scenario_name, test_name);
    else 
        log_fail("%s %s %s\n","[FAIL]", scenario_name, test_name);
    

    // Increment the pass/fail counter
    if (passed)
        ephemeral_passes++;
    else
        ephemeral_fails++;

    // Increment the test counter
    ephemeral_tests++;

    // Done
    return;
}

void print_final_summary ( void )
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // Print
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
    
    // Clear test counters for this test
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Done
    return;
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument checking 
    if ( path == 0 ) goto no_path;

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) 
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG

                #endif

            // Error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG

                #endif

            // Error
            return 0;
        }
    }
}