#ifndef YDS_NETWORKCONTROLLER_H
#define YDS_NETWORKCONTROLLER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <memory>
#include <QThread>
#include "RestApiWorker.h"


/*!
 \brief Maintains RestApiWorker working in isolated QThread
 \author Ddann
 \inherit QObject
 \details
 RestApiController create an instance of RestApiWorker and moves it to isolated QThread.
 It provides interaction with RestApiWorker
 by proxying requests to worker's slot through QueuedConnection.
 */
class RestApiController : public QObject {
Q_OBJECT
public:
  RestApiController(QObject *parent = nullptr);

  ~RestApiController();

public:
  /*!
   *\brief Sends HTTP GET request
   *\details proxying HTTP GET request to RestApiWorker, which
    * invokes recieve_slot with QNetworkReply* as argument when finished
   *
   * \param receiver request handler
   * \param recieve_slot normilized signature of handler slot
   * \param request request object to be sent
   *
   */

  void sendGetRequest(QObject *receiver,
                      const QByteArray &recieve_slot,
                      QNetworkRequest *request);

  /*!
   *\brief Sends HTTP POST request
   *\details proxying HTTP POST request to RestApiWorker, which
    * invokes recieve_slot with QNetworkReply* as argument when finished
   *
   * \param receiver request handler
   * \param recieve_slot normilized signature of handler slot
   * \param request request object to be sent
   * \param data pointer to date to be sent
   */

  void sendPostRequest(QObject *receiver,
                       const QByteArray &recieve_slot,
                       QNetworkRequest *request,
                       QByteArray *data);

  /*!
    *\brief Sends HTTP PUT request
    *\details proxying HTTP PUT request to RestApiWorker, which
    * invokes recieve_slot with QNetworkReply* as argument when finished
    *
    * \param receiver request handler
    * \param recieve_slot normilized signature of handler slot
    * \param request request object to be sent
    * \param data pointer to date to be sent
    */
  void sendPutRequest(QObject *receiver, const QByteArray &recieve_slot,
                      QNetworkRequest *request, QByteArray *data);

signals:

  /*!
    *\brief proxing a request with related information
    *
    * \param receiver request handler
    * \param recieve_slot normilized signature of handler slot
    * \param request request object to be sent
    * \param data pointer to date to be sent
    *
    */
  void proxyRequest(QObject *receiver,
                    const QByteArray &recieve_slot,
                    QNetworkRequest *request,
                    RequestType type,
                    QByteArray *data = nullptr);

private:
  RestApiWorker *worker;
  QThread worker_thread;
};


#endif //YDS_NETWORKCONTROLLER_H
