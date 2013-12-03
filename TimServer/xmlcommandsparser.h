#ifndef XMLCOMMANDSPARSER_H
#define XMLCOMMANDSPARSER_H

#include <QtXml/QDomDocument>
#include <QStringList>
#include <QSet>

class XMLCommandsParser
{
public:
    enum CommandType {
        CT_INIT,
        CT_USERS,
        CT_MESSAGE,
        CT_NAME,
        CT_ID
    };

    typedef QSet<CommandType> CommandsContainer;

    //-----------------------------------------------------------
    XMLCommandsParser(const CommandType& command);
    XMLCommandsParser(const CommandType& command,
                      int id);
    XMLCommandsParser(const CommandType& command,
                      const QStringList& users);
    XMLCommandsParser(const CommandType& command,
                      const QString& str);

    XMLCommandsParser(const QString& xml);
    //-----------------------------------------------------------

    void addCommand(const CommandType& command,
                    const QString& str);
    void addCommand(const CommandType& command,
                    int val);

    QString toString() const;
    QDomDocument toDocument() const;

    //-----------------------------------------------------------
    int getId() const { return id; }
    QString getMessage() const { return message; }
    QStringList getUsers() const { return users; }
    CommandsContainer getCommands() const { return commands; }

private:
    QString initCommands(const CommandType &command);
    QDomElement makeElement(const QString& strName,
                            const QString& strAttr = QString::null,
                            const QString& strText = QString::null);
    void traverseNode(const QDomNode& node);
private:
    QDomDocument doc;
    QDomElement  commandsElement;

    CommandsContainer commands;
    QStringList users;
    QString message;
    int id;
    QString name;

};

#endif // XMLCOMMANDSPARSER_H
