#include <QCoreApplication>

#include <Client>
#include <Processor>

class MyProcessor : public QJack::Processor {
public:
    MyProcessor(QJack::Port in, QJack::Port out)
        : Processor(),
          in(in),
          out(out) {

    }

    void process(int samples) {
        QJack::Buffer buffer = in.buffer(samples);
        buffer.multiply(0.5);
        buffer.copyTo(out.buffer(samples));
    }

private:
    QJack::Port& in;
    QJack::Port& out;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QJack::Client client;
    client.connectToServer("attenuator");

    MyProcessor processor(client.registerAudioInPort("in"),
                          client.registerAudioOutPort("out"));

    client.setProcessor(&processor);
    client.activate();

    return a.exec();
}
