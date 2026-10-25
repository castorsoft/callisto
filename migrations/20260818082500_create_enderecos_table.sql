-- Table: public.enderecos

-- DROP TABLE IF EXISTS public.enderecos;

CREATE TABLE IF NOT EXISTS public.enderecos
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    logradouro character varying(120) COLLATE pg_catalog."default" NOT NULL,
    numero character varying(20) COLLATE pg_catalog."default",
    complemento character varying(60) COLLATE pg_catalog."default",
    bairro character varying(60) COLLATE pg_catalog."default" NOT NULL,
    cep character(8) COLLATE pg_catalog."default" NOT NULL,
    id_cidade integer NOT NULL,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT enderecos_pk PRIMARY KEY (id),
    CONSTRAINT enderecos_unique UNIQUE (uuid),
    CONSTRAINT enderecos_cidades_fk FOREIGN KEY (id_cidade)
        REFERENCES public.cidades (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;