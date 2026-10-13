#include "DialogNovoProdutoRapido.h"
#include "ui_DialogNovoProdutoRapido.h"

#include "../../../infrastructure/database/repositories/ProdutoRepository.h"
#include "../../../infrastructure/database/repositories/UnidadeMedidaRepository.h"
#include "../../../infrastructure/database/repositories/NCMRepository.h"

#include <QMessageBox>
#include <QDate>

DialogNovoProdutoRapido::DialogNovoProdutoRapido(int idFornecedor,
                                                 const QString &/*nomeFornecedor*/,
                                                 const ItemNotaTerceiroInfo &itemInfo,
                                                 const QString &ncmSugerido,
                                                 QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogNovoProdutoRapido)
    , m_idFornecedor(idFornecedor)
    , m_itemInfo(itemInfo)
{
    ui->setupUi(this);

    ui->editDescricao->setText(itemInfo.descricao);
    ui->editCodigoInterno->setText(itemInfo.codigoProduto);
    ui->editCodigoBarra->setText(itemInfo.codigoBarras);
    ui->editNcm->setText(ncmSugerido);

    carregarCombos();

    connect(ui->btnSalvar, &QPushButton::clicked, this, &DialogNovoProdutoRapido::onSalvarClicked);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &QDialog::reject);
}

DialogNovoProdutoRapido::~DialogNovoProdutoRapido()
{
    delete ui;
}

void DialogNovoProdutoRapido::carregarCombos()
{
    // 1. Tipos de atividade
    ui->comboTipoAtividade->addItem("00 – Mercadoria para Revenda", TipoProdutoAtividade::MercadoriaRevenda);
    ui->comboTipoAtividade->addItem("01 – Matéria-Prima", TipoProdutoAtividade::MateriaPrima);
    ui->comboTipoAtividade->addItem("02 – Embalagem", TipoProdutoAtividade::Embalagem);
    ui->comboTipoAtividade->addItem("03 – Produto em Processo", TipoProdutoAtividade::ProdutoEmProcesso);
    ui->comboTipoAtividade->addItem("04 – Produto Acabado", TipoProdutoAtividade::ProdutoAcabado);
    ui->comboTipoAtividade->addItem("07 – Material de Uso e Consumo", TipoProdutoAtividade::MaterialUsoConsumo);
    ui->comboTipoAtividade->addItem("08 – Ativo Imobilizado", TipoProdutoAtividade::AtivoImobilizado);
    ui->comboTipoAtividade->addItem("09 – Serviços", TipoProdutoAtividade::Servicos);
    ui->comboTipoAtividade->addItem("99 – Outras", TipoProdutoAtividade::Outras);

    // 2. Unidades de medida
    UnidadeMedidaRepository umRepo;
    QList<UnidadeMedida> unidades = umRepo.listarTodas();

    int defaultIndex = -1;
    for (int i = 0; i < unidades.size(); ++i) {
        const auto &um = unidades[i];
        ui->comboUnidade->addItem(QString("%1 - %2").arg(um.sigla(), um.descricao()), um.id().value_or(0));

        if (defaultIndex == -1 && um.sigla().compare(m_itemInfo.unidadeMedida, Qt::CaseInsensitive) == 0) {
            defaultIndex = i;
        }
    }

    if (defaultIndex >= 0) {
        ui->comboUnidade->setCurrentIndex(defaultIndex);
    }
}

int DialogNovoProdutoRapido::obterOuCriarUnidadeMedida(const QString &sigla)
{
    UnidadeMedidaRepository umRepo;
    QString siglaLimpa = sigla.trimmed().toUpper();
    if (siglaLimpa.isEmpty()) siglaLimpa = "UN";

    auto optUm = umRepo.buscarPorSigla(siglaLimpa);
    if (optUm.has_value()) {
        return optUm.value().id().value();
    }

    UnidadeMedida novaUm(siglaLimpa, siglaLimpa);
    if (umRepo.salvar(novaUm)) {
        return novaUm.id().value();
    }
    return 1;
}

int DialogNovoProdutoRapido::obterOuCriarNCM(const QString &codigo)
{
    NCMRepository ncmRepo;
    QString codLimpo = codigo;
    codLimpo = codLimpo.remove('.').remove('-').remove(' ').trimmed();

    if (!codLimpo.isEmpty()) {
        auto optNcm = ncmRepo.buscarPorCodigo(codLimpo);
        if (optNcm.has_value()) {
            return optNcm.value().id().value();
        }

        NCM novoNcm(codLimpo, "CADASTRO AUTOMATICO", 0, 0, 0, QDate::currentDate(), QDate::currentDate().addYears(1));
        if (ncmRepo.salvar(novoNcm)) {
            return novoNcm.id().value();
        }
    }

    // Fallback: NCM padrão geral
    auto optPadrao = ncmRepo.buscarPorCodigo("00000000");
    if (optPadrao.has_value()) {
        return optPadrao.value().id().value();
    }

    NCM ncmGenerico("00000000", "OUTROS", 0, 0, 0, QDate::currentDate(), QDate::currentDate().addYears(5));
    ncmRepo.salvar(ncmGenerico);
    return ncmGenerico.id().value_or(1);
}

void DialogNovoProdutoRapido::onSalvarClicked()
{
    QString descricao = ui->editDescricao->text().trimmed();
    if (descricao.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "A descrição do produto é obrigatória.");
        ui->editDescricao->setFocus();
        return;
    }

    int idUm = ui->comboUnidade->currentData().toInt();
    if (idUm <= 0) {
        idUm = obterOuCriarUnidadeMedida(m_itemInfo.unidadeMedida);
    }

    int idNcm = obterOuCriarNCM(ui->editNcm->text());
    QString tipoAtividade = ui->comboTipoAtividade->currentData().toString();

    std::optional<QString> codBarra;
    QString cb = ui->editCodigoBarra->text().trimmed();
    if (!cb.isEmpty()) codBarra = cb;

    std::optional<QString> codInterno;
    QString ci = ui->editCodigoInterno->text().trimmed();
    if (!ci.isEmpty()) codInterno = ci;

    Produto novoProduto(descricao, idUm, idNcm, tipoAtividade, codBarra, codInterno, std::nullopt);
    ProdutoRepository prodRepo;

    if (!prodRepo.salvar(novoProduto) || !novoProduto.id().has_value()) {
        QMessageBox::critical(this, "Erro", "Não foi possível cadastrar o produto no banco de dados.");
        return;
    }

    m_idProdutoCriado = novoProduto.id().value();
    accept();
}

double DialogNovoProdutoRapido::multiplicador() const
{
    return ui->spinMultiplicador->value();
}

int DialogNovoProdutoRapido::idUnidadeMedida() const
{
    return ui->comboUnidade->currentData().toInt();
}

