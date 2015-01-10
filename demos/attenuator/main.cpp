#include <QCoreApplication>
#include <QDebug>

#include <Client>
#include <Processor>
#include <NetMaster>
#include <NetSlave>

class MyProcessor : public QJack::Processor {
public:
    MyProcessor(QJack::Client& client)
        : Processor(client)  {
        in = client.registerAudioInPort("in");
        out = client.registerAudioOutPort("out");
        memoryBuffer = QJack::Buffer::createMemoryAudioBuffer(4096);
    }

    void process(int samples) {
        // Copy from input buffer to memory buffer
        in.buffer(samples).copyTo(memoryBuffer);

        // Process
        memoryBuffer.multiply(0.5);

        // Copy result to output buffer
        memoryBuffer.copyTo(out.buffer(samples));
    }

private:
    QJack::Port in;
    QJack::Port out;

    QJack::Buffer memoryBuffer;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QJack::Client client;
    client.connectToServer("attenuator_demo");

    MyProcessor processor(client);
    client.activate();

    QJack::NetMaster netMaster;
    bool s = netMaster.open("localhost", 9090, "test");
qDebug() << s;
    return a.exec();
}
