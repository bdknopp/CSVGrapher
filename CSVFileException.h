/*
 * CSVFileException.h: Supporting exception class for CSV file
 *                   : reading/writing.
 * Author: B. D. Knopp: bdknopp@users.noreply.github.com
 * Version: 1.00: Initial implementation.
 * Date: 23 August 2014
 */

#ifndef CSVFILEEXCEPTION_H
#define CSVFILEEXCEPTION_H

/*
 * Class: CSVFileException
 * Description: Exception class for reading/writing CSV files.
 */
class CSVFileException : public std::exception
{
  /* Public methods. */
  public:
    /*
     * Constructor: CSVFileException
     * Description: Creates a new file exception with given message.
     * Parameters: message: Message to store.
     */
    CSVFileException(std::string message) : msg(message) { }

    /*
     * Destructor: ~CSVFileException
     * Description: Destroys an instance of the exception.
     */
    ~CSVFileException() throw() { }

    /*
     * Method: what
     * Description: Retrieves the associated exception message.
     * Parameters: none.
     * Returns: Exception message.
     */
    const char* what() const throw() { return msg.c_str(); }

  /* Private members. */
  private:
    std::string msg;
};

#endif // CSVFILEEXCEPTION_H
