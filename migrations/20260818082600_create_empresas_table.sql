-- Table: public.empresas

-- DROP TABLE IF EXISTS public.empresas;

CREATE TABLE IF NOT EXISTS public.empresas
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    cnpj_cpf character varying(14) COLLATE pg_catalog."default" NOT NULL,
    razao_social character varying(120) COLLATE pg_catalog."default" NOT NULL,
    nome_fantasia character varying(120) COLLATE pg_catalog."default",
    regime_tributario character varying(20) COLLATE pg_catalog."default" NOT NULL,
    id_endereco integer NOT NULL,
    inscricao_estadual character varying(16) COLLATE pg_catalog."default",
    inscricao_municipal character varying(16) COLLATE pg_catalog."default",
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT empresas_pk PRIMARY KEY (id),
    CONSTRAINT empresas_unique UNIQUE (uuid),
    CONSTRAINT empresas_unique_1 UNIQUE (cnpj_cpf),
    CONSTRAINT empresas_enderecos_fk FOREIGN KEY (id_endereco)
        REFERENCES public.enderecos (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;