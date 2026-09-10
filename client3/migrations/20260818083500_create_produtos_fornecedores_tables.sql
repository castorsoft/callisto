-- Table: public.produtos_fornecedores

-- DROP TABLE IF EXISTS public.produtos_fornecedores;

CREATE TABLE IF NOT EXISTS public.produtos_fornecedores
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    id_produto integer NOT NULL,
    id_fornecedor integer NOT NULL,
    codigo_fornecedor character varying COLLATE pg_catalog."default" NOT NULL,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    id_unidade_medida integer NOT NULL,
    multiplicador double precision NOT NULL,
    codigo_barra_caixa character varying(20) COLLATE pg_catalog."default",
    CONSTRAINT produtos_fornecedores_pkey PRIMARY KEY (id_produto),
    CONSTRAINT fk_produtos_fornecedores_fornecedor FOREIGN KEY (id_fornecedor)
        REFERENCES public.pessoas_fornecedor (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT fk_produtos_fornecedores_produto FOREIGN KEY (id_produto)
        REFERENCES public.produtos (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT fk_produtos_fornecedores_unidade_medida FOREIGN KEY (id_unidade_medida)
        REFERENCES public.unidades_medida (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;
