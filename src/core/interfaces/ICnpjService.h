#pragma once

#include <QObject>
#include <QString>
#include <QMetaType>
#include <functional>

struct CnpjResult {
    QString cnpj;
    QString razaoSocial;
    QString nomeFantasia;
    QString regimeTributario;

    QString logradouro;
    QString numero;
    QString complemento;
    QString bairro;
    QString cep;
    int ibgeCidade = 0;
    QString siglaEstado;

    QString inscricaoEstadual;
    bool isValid = false;
    QString errorString;
};

Q_DECLARE_METATYPE(CnpjResult)

class ICnpjService : public QObject {
    Q_OBJECT
public:
    using Factory = std::function<ICnpjService*(QObject*)>;

    explicit ICnpjService(QObject *parent = nullptr) : QObject(parent) {}
    ~ICnpjService() override = default;

    virtual void consultarCNPJ(const QString &cnpj) = 0;

    static void setFactory(Factory factory) {
        s_factory = std::move(factory);
    }

    static ICnpjService* create(QObject *parent = nullptr) {
        if (s_factory) {
            return s_factory(parent);
        }
        return nullptr;
    }

signals:
    void consultaFinalizada(const CnpjResult &resultado);
    void erroConsulta(const QString &erro);

private:
    inline static Factory s_factory = nullptr;
};

