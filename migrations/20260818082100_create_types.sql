-- Type: tipo_contato

-- DROP TYPE IF EXISTS public.tipo_contato;

CREATE TYPE public.tipo_contato AS ENUM
    ('email', 'telefone', 'celular');

-- Type: tipo_pessoa_alteracao

-- DROP TYPE IF EXISTS public.tipo_pessoa_alteracao;

CREATE TYPE public.tipo_pessoa_alteracao AS ENUM
    ('cnpj_cpf', 'nome', 'fantasia', 'id_endereco', 'inscricao_estadual');

-- Type: tipo_produto_alteracao

-- DROP TYPE IF EXISTS public.tipo_produto_alteracao;

CREATE TYPE public.tipo_produto_alteracao AS ENUM
    ('descricao', 'codigo_barra', 'id_unidade_medida', 'tipo_atividade', 'id_ncm', 'codigo_interno');

-- Type: tipo_produto_atividade

-- DROP TYPE IF EXISTS public.tipo_produto_atividade;

CREATE TYPE public.tipo_produto_atividade AS ENUM
    ('00', '01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '99');

-- Type: tipo_produto_tributacao

-- DROP TYPE IF EXISTS public.tipo_produto_tributacao;

CREATE TYPE public.tipo_produto_tributacao AS ENUM
    ('icms', 'ipi', 'pis', 'cofins');
