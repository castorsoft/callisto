-- Table: public.pessoas

-- DROP TABLE IF EXISTS public.pessoas;

CREATE TABLE IF NOT EXISTS public.pessoas
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    cnpj_cpf character varying(14) COLLATE pg_catalog."default",
    nome character varying(120) COLLATE pg_catalog."default" NOT NULL,
    fantasia character varying(120) COLLATE pg_catalog."default",
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    id_endereco integer,
    inscricao_estadual character varying(20) COLLATE pg_catalog."default",
    CONSTRAINT pessoas_pkey PRIMARY KEY (id),
    CONSTRAINT pessoas_endereco FOREIGN KEY (id_endereco)
        REFERENCES public.enderecos (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;
