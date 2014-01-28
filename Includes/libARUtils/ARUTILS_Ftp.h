/**
 * @file ARUTILS_Ftp.h
 * @brief libARUtils Ftp header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARUTILS_FTP_H_
#define _ARUTILS_FTP_H_

#include <inttypes.h>
#include <libARSAL/ARSAL_Sem.h>
#include "libARUtils/ARUTILS_Error.h" 

/**
 * @brief Ftp anonymous user name
 */
#define ARUTILS_FTP_ANONYMOUS        "anonymous"

/**
 * @brief Ftp max url string size
 */
#define ARUTILS_FTP_MAX_URL_SIZE      512

/**
 * @brief Ftp max file name path string size
 */
#define ARUTILS_FTP_MAX_PATH_SIZE     256

/**
 * @brief Ftp max list line string size
 */
#define ARUTILS_FTP_MAX_LIST_LINE_SIZE     512

/**
 * @brief Ftp Resume enum
 * @see ARUTILS_Ftp_Get
 */
typedef enum
{
    FTP_RESUME_FALSE = 0,
    FTP_RESUME_TRUE,
    
} eARUTILS_FTP_RESUME;

/**
 * @brief Ftp Connection structure
 * @see ARUTILS_Ftp_NewConnection
 */
typedef struct ARUTILS_Ftp_Connection_t ARUTILS_Ftp_Connection_t;

/**
 * @brief Progress callback of the Ftp download
 * @param arg The pointer of the user custom argument
 * @param percent The percent size of the media file already downloaded
 * @see ARUTILS_Ftp_Get ()
 */
typedef void (*ARUTILS_Ftp_ProgressCallback_t)(void* arg, uint8_t percent);

/**
 * @brief Create a new Ftp Connection
 * @warning This function allocates memory
 * @param cancelSem The pointer of the Ftp get/put cancel semaphore or null
 * @param server The Ftp server IP address
 * @param port The Ftp server port
 * @param username The Ftp server account name
 * @param password The Ftp server account password
 * @param[out] error The pointer of the error code: if success ARUTILS_OK, otherwise an error number of eARUTILS_ERROR
 * @retval On success, returns an ARUTILS_Ftp_Connection_t. Otherwise, it returns null.
 * @see ARUTILS_Ftp_DeleteConnection ()
 */
ARUTILS_Ftp_Connection_t * ARUTILS_Ftp_Connection_New(ARSAL_Sem_t *cancelSem, const char *server, int port, const char *username, const char* password, eARUTILS_ERROR *error);

/**
 * @brief Delete an Ftp Connection
 * @warning This function frees memory
 * @param connection The address of the pointer on the Ftp Connection
 * @see ARUTILS_Ftp_NewConnection ()
 */
void ARUTILS_Ftp_Connection_Delete(ARUTILS_Ftp_Connection_t **connection);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_Ftp_Connection_Cancel(ARUTILS_Ftp_Connection_t *connection);

/**
 * @brief Check if the connection has received a cancel to it's semaphore
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_Ftp_IsCanceled(ARUTILS_Ftp_Connection_t *connection);

/**
 * @brief Execute Ftp List command to retrieve directory content
 * @warning This function allocates memory
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the directory path on the remote Ftp server
 * @param resultList The pointer of the string of the directory content null terminated
 * @param resultListLen The pointer of the lenght of the resultList string including null terminated
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_Ftp_List(ARUTILS_Ftp_Connection_t *connection, const char *namePath, char **resultList, uint32_t *resultListLen);

/**
 * @brief File list iterator function
 * @param list The file list
 * @param nextItem The the next file
 * @param prefix The file prefix to match
 * @param isDirectory The file type requested: 1 directory or 0 file
 * @param indexItem The beginning of the line item if address is not null
 * @param itemLen The length of the line item if address is not null
 * @retval On success, returns the file name of the found item, Otherwise, it returns null
 * @see ARUTILS_Ftp_List ()
 */
const char * ARUTILS_Ftp_List_GetNextItem(const char *list, const char **nextItem, const char *prefix, int isDirectory, const char **indexItem, int *itemLen);

/**
 * @brief File size accessor function from a file of a File list
 * @param line The line of the File list wehre to search file size
 * @param lineSize The size of the given line
 * @param size The addresse of the pointer to the file size to return
 * @retval On success, returns the addresse found size string, Otherwise, it returns null
 * @see ARUTILS_Ftp_List_GetNextItem ()
*/
const char * ARUTILS_Ftp_List_GetItemSize(const char *line, int lineSize, double *size);

/**
 * @brief Rename an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param oldNamePath The string of the old file name path on the remote Ftp server
 * @param newNamePath The string of the new file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_Ftp_Rename(ARUTILS_Ftp_Connection_t *connection, const char *oldNamePath, const char *newNamePath);

/**
 * @brief Get the size of an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_Ftp_Size(ARUTILS_Ftp_Connection_t *connection, const char *namePath, double *size);

/**
 * @brief Delete an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_Ftp_Delete(ARUTILS_Ftp_Connection_t *connection, const char *namePath);


/**
 * @brief Get an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param dstFile The string of the local file name path to be get
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_Ftp_Get(ARUTILS_Ftp_Connection_t *connection, const char *namePath, const char *dstFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Get an remote Ftp server file
 * @warning This function allocates memory
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param data The pointer of the data buffer of the file data
 * @param dataLen The pointer of the length of the data buffer
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_Ftp_Get_WithBuffer(ARUTILS_Ftp_Connection_t *connection, const char *namePath, uint8_t **data, uint32_t *dataLen,  ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Put an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param srcFile The string of the local file name path to be put
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_Ftp_Put(ARUTILS_Ftp_Connection_t *connection, const char *namePath, const char *srcFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

#endif /* _ARUTILS_FTP_H_ */
