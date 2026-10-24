-- Migration para ajustar chaves primárias (PKs) e chaves estrangeiras (FKs) pendentes no banco de dados

-- 1. Tabela documentos_fiscais: Adicionar FK id_pessoa -> public.pessoas(id)
ALTER TABLE public.documentos_fiscais
DROP CONSTRAINT IF EXISTS fk_documentos_fiscais_pessoa;

ALTER TABLE public.documentos_fiscais
ADD CONSTRAINT fk_documentos_fiscais_pessoa
FOREIGN KEY (id_pessoa) REFERENCES public.pessoas (id)
ON UPDATE NO ACTION ON DELETE RESTRICT;

-- 2. Tabela empresas: Corrigir FK id_endereco (apontava incorretamente para cidades em vez de enderecos)
ALTER TABLE public.empresas
DROP CONSTRAINT IF EXISTS empresas_cidades_fk;

ALTER TABLE public.empresas
DROP CONSTRAINT IF EXISTS empresas_enderecos_fk;

ALTER TABLE public.empresas
ADD CONSTRAINT empresas_enderecos_fk
FOREIGN KEY (id_endereco) REFERENCES public.enderecos (id)
ON UPDATE NO ACTION ON DELETE RESTRICT;

-- 3. Tabela produtos_fornecedores: Ajustar PK em 'id', FK id_fornecedor -> pessoas(id) e UNIQUE
ALTER TABLE public.produtos_fornecedores
DROP CONSTRAINT IF EXISTS fk_produtos_fornecedores_fornecedor;

ALTER TABLE public.produtos_fornecedores
ADD CONSTRAINT fk_produtos_fornecedores_fornecedor
FOREIGN KEY (id_fornecedor) REFERENCES public.pessoas (id)
ON UPDATE NO ACTION ON DELETE CASCADE;

ALTER TABLE public.produtos_fornecedores
DROP CONSTRAINT IF EXISTS produtos_fornecedores_pkey;

ALTER TABLE public.produtos_fornecedores
ADD CONSTRAINT produtos_fornecedores_pkey PRIMARY KEY (id);

ALTER TABLE public.produtos_fornecedores
DROP CONSTRAINT IF EXISTS unq_produtos_fornecedores_fornecedor_codigo;

ALTER TABLE public.produtos_fornecedores
ADD CONSTRAINT unq_produtos_fornecedores_fornecedor_codigo
UNIQUE (id_fornecedor, codigo_fornecedor);

-- 4. Tabela empresa_contatos: Definir PK no campo 'id' e UNIQUE em (id_empresa, id_contato)
ALTER TABLE public.empresa_contatos
DROP CONSTRAINT IF EXISTS empresa_contatos_pkey;

ALTER TABLE public.empresa_contatos
ADD CONSTRAINT empresa_contatos_pkey PRIMARY KEY (id);

ALTER TABLE public.empresa_contatos
DROP CONSTRAINT IF EXISTS unq_empresa_contatos;

ALTER TABLE public.empresa_contatos
ADD CONSTRAINT unq_empresa_contatos UNIQUE (id_empresa, id_contato);

-- 5. Tabela pessoas_alteracoes: Definir PK no campo 'id'
ALTER TABLE public.pessoas_alteracoes
DROP CONSTRAINT IF EXISTS pessoas_alteracoes_pkey;

ALTER TABLE public.pessoas_alteracoes
ADD CONSTRAINT pessoas_alteracoes_pkey PRIMARY KEY (id);

-- 6. Tabela pessoas_contabilista: Definir PK no campo 'id' e UNIQUE em id_pessoa
ALTER TABLE public.pessoas_contabilista
DROP CONSTRAINT IF EXISTS pessoas_contabilista_pkey;

ALTER TABLE public.pessoas_contabilista
ADD CONSTRAINT pessoas_contabilista_pkey PRIMARY KEY (id);

ALTER TABLE public.pessoas_contabilista
DROP CONSTRAINT IF EXISTS unq_pessoas_contabilista_pessoa;

ALTER TABLE public.pessoas_contabilista
ADD CONSTRAINT unq_pessoas_contabilista_pessoa UNIQUE (id_pessoa);

-- 7. Tabela produtos_tributacoes: Definir PK no campo 'id'
ALTER TABLE public.produtos_tributacoes
DROP CONSTRAINT IF EXISTS produtos_tributacoes_pkey;

ALTER TABLE public.produtos_tributacoes
ADD CONSTRAINT produtos_tributacoes_pkey PRIMARY KEY (id);

