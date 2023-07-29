#include "connect.h"

class ConnectData : public QSharedData
{
public:
};

Connect::Connect() : data(new ConnectData)
{

}

Connect::Connect(const Connect &rhs) : data(rhs.data)
{

}

Connect &Connect::operator=(const Connect &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Connect::~Connect()
{

}
