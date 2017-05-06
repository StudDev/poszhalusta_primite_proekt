#include "RestApiBase.h"

ReplyWrapper *RestApiBase::get(const QUrl &request, QUrlQuery params) {
  auto target = QUrl(request);
  target.setQuery(params);
  auto* reply = new ReplyWrapper(oauth->get(request));
  return reply;
}

ReplyWrapper *RestApiBase::deleteResource(const QUrl &request, QUrlQuery params) {
  return nullptr;
}

ReplyWrapper *RestApiBase::post(const QUrl &request, QUrlQuery params) {
  return nullptr;
}

ReplyWrapper *RestApiBase::put(const QUrl &request, QUrlQuery params) {
  return nullptr;
}

RestApiBase::TokenState RestApiBase::checkTokenState() {
  return TokenState::Empty;
}

RestApiBase::RestApiBase(QNetworkAccessManager *manager, QObject *parent)
  :QObject(parent), oauth(new QOAuth2AuthorizationCodeFlow(manager, this)){
  if(checkTokenState() != TokenState::Normal){
    grantAccess();
  }
}

RestApiBase::RestApiBase(QObject *parent)
  :QObject(parent),
   oauth(new QOAuth2AuthorizationCodeFlow(new QNetworkAccessManager(this), this)) {
  if(checkTokenState() != TokenState::Normal){
    grantAccess();
  }
}

void RestApiBase::grantAccess() {
  AuthorizationController authorizer(oauth,this);
  authorizer.grant();
  oauth = authorizer.getOAuth2AuthorizationCodeFlow();
  oauth->setParent(this);
}
