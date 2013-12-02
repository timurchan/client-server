#include "xmlcommandsparser.h"

static QString commandType2String(const XMLCommandsParser::CommandType& command)
{
    switch (command) {
    case XMLCommandsParser::CT_INIT:
        return "init";
    case XMLCommandsParser::CT_USERS:
        return "users";
    case XMLCommandsParser::CT_MESSAGE:
        return "message";
    default:
        return "unknkown_value";
    }
}

static XMLCommandsParser::CommandType string2CommandType(const QString& str)
{
    if(str == "init") {
        return XMLCommandsParser::CT_INIT;
    } else if(str == "users") {
        return XMLCommandsParser::CT_USERS;
    } else if(str == "message") {
        return XMLCommandsParser::CT_MESSAGE;
    }
}

QDomElement XMLCommandsParser::makeElement(const QString& strName,
                                            const QString& strAttr,
                                            const QString& strText)
{
    QDomElement domElement = doc.createElement(strName);

    if (!strAttr.isEmpty()) {
        QDomAttr domAttr = doc.createAttribute("type");
        domAttr.setValue(strAttr);
        domElement.setAttributeNode(domAttr);
    }

    if (!strText.isEmpty()) {
        QDomText domText = doc.createTextNode(strText);
        domElement.appendChild(domText);
    }
    return domElement;
}

QString XMLCommandsParser::initCommands(const CommandType &command)
{
    commandsElement = doc.createElement("commands");
    doc.appendChild(commandsElement);
    QString strCommand = commandType2String(command);
    return strCommand;
}

XMLCommandsParser::XMLCommandsParser(const CommandType &command)
{
    QString strCommand = initCommands(command);
    QDomElement oneCommand = makeElement("command", strCommand);
    commandsElement.appendChild(oneCommand);
}

XMLCommandsParser::XMLCommandsParser(const CommandType &command,
                                     const QStringList& users)
{
    QString strCommand = initCommands(command);
    QDomElement oneCommand = makeElement("command", strCommand);

    foreach (const QString& user, users) {
        oneCommand.appendChild(makeElement("user", "", user));
    }

    commandsElement.appendChild(oneCommand);
}

XMLCommandsParser::XMLCommandsParser(const CommandType &command,
                                     const QString& message)
{
    QString strCommand = initCommands(command);
    QDomElement oneCommand = makeElement("command", strCommand, message);
    commandsElement.appendChild(oneCommand);
}

XMLCommandsParser::XMLCommandsParser(const CommandType &command,
                                     int port)
{
    QString strCommand = initCommands(command);
    QDomElement oneCommand = makeElement("command", strCommand, QString::number(port));
    commandsElement.appendChild(oneCommand);
}

XMLCommandsParser::XMLCommandsParser(const QString& xml)
{
    if(doc.setContent(xml)) {
        QDomElement domElement= doc.documentElement();
        traverseNode(domElement);
    }
}

void XMLCommandsParser::traverseNode(const QDomNode& node)
{
   QDomNode domNode = node.firstChild();
   while(!domNode.isNull()) {
       if(domNode.isElement()) {
          QDomElement domElement = domNode.toElement();
          if(!domElement.isNull()) {
              if(domElement.tagName() == "command") {
                  QString attrName = domElement.attribute("type", "");
                  CommandType cmd = string2CommandType(attrName);
                  commands.insert(cmd);
                  if(attrName == "message") {
                      message = domElement.text();
                  }
                  if(attrName == "init") {
                      port = domElement.text().toInt();
                  }
              } else if(domElement.tagName() == "user"){
                  users << domElement.text();
              }
          }
       }
       traverseNode(domNode);
       domNode = domNode.nextSibling();
    }
}

QString XMLCommandsParser::toString() const
{
    return doc.toString();
}

QDomDocument XMLCommandsParser::toDocument() const
{
    return doc;
}
