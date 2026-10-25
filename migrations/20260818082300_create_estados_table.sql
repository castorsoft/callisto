-- Table: public.estados

-- DROP TABLE IF EXISTS public.estados;

CREATE TABLE IF NOT EXISTS public.estados
(
    id SERIAL,
    nome character varying(100) COLLATE pg_catalog."default" NOT NULL,
    sigla character(2) COLLATE pg_catalog."default" NOT NULL,
    id_ibge smallint NOT NULL,
    CONSTRAINT uf_pk PRIMARY KEY (id),
    CONSTRAINT uf_unique UNIQUE (sigla)
)

TABLESPACE pg_default;