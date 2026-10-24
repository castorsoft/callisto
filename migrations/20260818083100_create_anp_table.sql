-- Table: public.anp

-- DROP TABLE IF EXISTS public.anp;

CREATE TABLE IF NOT EXISTS public.anp
(
    id SERIAL,
    codigo character varying(15) COLLATE pg_catalog."default" NOT NULL,
    descricao character varying(120) COLLATE pg_catalog."default" NOT NULL,
    CONSTRAINT anp_pkey PRIMARY KEY (id),
    CONSTRAINT unq_anp_codigo UNIQUE (codigo)
)

TABLESPACE pg_default;