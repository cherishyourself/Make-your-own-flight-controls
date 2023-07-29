#ifndef CONNECT_H
#define CONNECT_H

#include <QObject>
#include <QSharedDataPointer>
#include <QWidget>

class ConnectData;

class Connect
{
    Q_OBJECT
public:
    Connect();
    Connect(const Connect &);
    Connect &operator=(const Connect &);
    ~Connect();

private:
    QSharedDataPointer<ConnectData> data;
};

#endif // CONNECT_H
