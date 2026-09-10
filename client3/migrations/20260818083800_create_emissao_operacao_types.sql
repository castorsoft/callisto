-- Type: tipo_dfe_emissao

-- DROP TYPE IF EXISTS public.tipo_dfe_emissao;

CREATE TYPE public.tipo_dfe_emissao AS ENUM
    ('propria', 'terceiro');

-- Type: tipo_dfe_operacao

-- DROP TYPE IF EXISTS public.tipo_dfe_operacao;

CREATE TYPE public.tipo_dfe_operacao AS ENUM
    ('saida', 'entrada');
