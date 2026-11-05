# Callisto DFE

O **Callisto DFE** é uma aplicação desktop desenvolvida em **C++17** e **Qt 6** com persistência em **PostgreSQL**, projetada para centralizar a gestão de Documentos Fiscais Eletrônicos (NF-e), automatizar a recepção e conciliação de mercadorias de terceiros e gerar obrigações acessórias fiscais — como o **SPED Fiscal (EFD ICMS/IPI)** — através de uma arquitetura extensível baseada em plugins.

---

## 💡 Proposta do Projeto

O **Callisto DFE** oferece:

1. **Ingestão e Processamento em Lote de XMLs (NF-e):**
   - Leitura assíncrona e paralela (`QThreadPool`) de diretórios e subpastas contendo arquivos XML, utilizando a biblioteca **Zafra** para decodificação.
   - Identificação automática de notas de **Emissão Própria** vs. **Emissão de Terceiros** com base na empresa ativa.
   - Cadastro automático de participantes (fornecedores/destinatários), unidades de medida, NCMs e produtos, preservando o XML original íntegro no banco de dados (`documentos_fiscais_arquivos`).
2. **Conciliação e Vínculo de Produtos de Terceiros (De-Para):**
   - Associação automática via código de barras (`<cEAN>` / GTIN) durante a importação.
   - Painel dedicado para conciliação manual de itens pendentes, permitindo vincular a produtos existentes (com fator multiplicador de conversão) ou cadastrar novos produtos aproveitando os dados do próprio XML.
3. **Cadastros Base Integrados:**
   - Gestão de **Empresas**, **Pessoas** (Clientes, Fornecedores e Contabilistas) e **Produtos**, com preenchimento automático de dados cadastrais e endereço via consulta de CNPJ, além de buscas interativas em tempo real (`InputLookup` e filtros multicoluna nas listagens).
4. **Arquitetura de Plugins & SPED Fiscal (EFD ICMS/IPI):**
   - Suporte ao carregamento dinâmico de plugins (`.so` / `.dll`) que estendem a interface principal (`MainWindow`) consumindo repositórios de domínio e componentes visuais padronizados via `IPluginContext`.
   - Plugin **SPED Fiscal** estruturado em 3 etapas desacopladas (**Coleta em Memória** → **Mapeamento de Blocos 0, C, vazios e 9** → **Serialização `.txt`**) pronto para validação no PVA da Receita Federal.

---

## 🛠️ Pré-requisitos de Ambiente

Para compilar e executar o projeto, certifique-se de ter instalado no sistema:

- **Compilador C++17:** GCC (`g++` 9+) ou Clang
- **CMake:** versão `3.19` ou superior
- **Ninja** ou **Make** (sistema de build)
- **Qt 6 (6.5+):** com os módulos:
  - `Core`, `Gui`, `Widgets`, `Network`, `Concurrent`, `Sql`
  - Driver SQL do PostgreSQL para Qt (`QPSQL`)
- **PostgreSQL:** versão 13 ou superior
- **Biblioteca Zafra:** Disponível [aqui](https://github.com/castorsoft/zafra).

### Instalação de dependências (Linux)

```bash
# Debian / Ubuntu
sudo apt update
sudo apt install build-essential cmake ninja-build qt6-base-dev libqt6sql6-psql libpq-dev libxml2-dev

# Arch Linux
sudo pacman -S base-devel cmake ninja qt6-base postgresql-libs libxml2
```

---

## ⚙️ Configuração Antes de Executar

### 1. Estrutura esperada de diretórios

O `CMakeLists.txt` referencia a biblioteca **Zafra** no caminho relativo `../zafra`:

```text
castorsoft/
├── zafra/                  # Biblioteca de parsing de documentos fiscais (NF-e)
└── dfe/                    # Este repositório (Callisto DFE)
```

### 2. Banco de Dados e `config.json`

Crie um banco de dados vazio no PostgreSQL (por padrão chamado `callisto`).
Na raiz do projeto (copiado automaticamente para a pasta `build/` pelo CMake, ou criado com valores padrão no primeiro uso), configure o arquivo `config.json`:

```json
{
  "database": {
    "host": "localhost",
    "port": 5432,
    "database": "callisto",
    "user": "postgres",
    "password": "secret"
  }
}
```

> **Nota:** Ao iniciar a aplicação, o `MigrationRunner` executará automaticamente todas as *migrations* SQL contidas na pasta `migrations/` na ordem cronológica.

---

## 🚀 Como Fazer o Build da Aplicação

### Opção 1: Via Terminal (CMake + Ninja/Make)

1. Acesse a pasta raiz do projeto:
   ```bash
   cd /caminho/para/castorsoft/dfe
   ```

2. Configure o diretório de build:
   ```bash
   cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Release
   ```
   *(Caso não utilize o Ninja, basta omitir `-GNinja` para utilizar o `Makefile` padrão).*

3. Compile o executável principal (`callisto`), a biblioteca `core_lib` e todos os plugins:
   ```bash
   cmake --build build --parallel
   ```

4. Para compilar apenas um alvo específico (ex.: o plugin do SPED Fiscal):
   ```bash
   cmake --build build --target sped_fiscal
   ```

5. Execute a aplicação:
   ```bash
   ./build/callisto
   ```

---

### Opção 2: Via Qt Creator

1. Abra o **Qt Creator** e selecione **File > Open File or Project...**.
2. Selecione o arquivo `CMakeLists.txt` na raiz de `dfe/`.
3. Escolha o **Kit Desktop Qt 6** desejado e clique em **Configure Project**.
4. Pressione `Ctrl + B` (Build) para compilar o executável `callisto` e os plugins (`external_plugins/sped_fiscal`, `external_plugins/test_plugin` e `qt_plugins`).
5. Pressione `Ctrl + R` (Run) para iniciar a aplicação.

---

## 📂 Estrutura do Projeto

```text
dfe/
├── CMakeLists.txt              # Configuração principal de build CMake
├── config.json                 # Parâmetros de conexão com o PostgreSQL
├── docs/                       # Documentação e especificação de requisitos
├── migrations/                 # Scripts SQL versionados executados na inicialização
├── qt_plugins/                 # Plugins de componentes customizados para o Qt Designer
├── external_plugins/           # Plugins dinâmicos carregados em runtime (bin/plugins)
│   ├── sped_fiscal/            # Plugin de geração do SPED Fiscal (EFD ICMS/IPI)
│   └── test_plugin/            # Plugin de exemplo/teste
└── src/
    ├── main.cpp                # Ponto de entrada, SplashScreen e inicialização
    ├── core/                   # Entidades de domínio, interfaces de repositórios, contratos de plugins (IPlugin, IPluginContext) e constantes
    ├── application/            # Casos de uso (ex.: ProcessFile)
    ├── infrastructure/         # PostgreSQL, Repositórios SQL, Migrations, CnpjService e PluginManager
    └── presentation/           # Views (MainWindow, Forms), Providers, Inputs customizados e contratos de UI de plugins (IUIExtension, IUIPluginContext)
```
