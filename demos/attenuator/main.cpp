#include <QCoreApplication>

#include <Client>
#include <Processor>
#include <RingBuffer>

class MyProcessor : public QJack::Processor {
public:
    MyProcessor(QJack::Client& client)
        : Processor(client)  {
        in = client.registerAudioInPort("in");
        out = client.registerAudioOutPort("out");
        ringBuffer = QJack::AudioRingBuffer();
    }

    void process(int samples) {
        in.buffer(samples).push(ringBuffer);
        out.buffer(samples).pop(ringBuffer);
        out.buffer(samples).multiply(0.5);
    }

private:
    QJack::AudioPort in;
    QJack::AudioPort out;
    QJack::AudioRingBuffer ringBuffer;
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
