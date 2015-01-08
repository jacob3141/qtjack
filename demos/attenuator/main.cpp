#include <QCoreApplication>

#include <Client>
#include <Processor>

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

    return a.exec();
}
