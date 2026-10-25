-- PROCEDURE: public.pr_criar_produto_alteracao(integer, tipo_produto_alteracao, character varying, character varying)

-- DROP PROCEDURE IF EXISTS public.pr_criar_produto_alteracao(integer, tipo_produto_alteracao, character varying, character varying);

CREATE OR REPLACE PROCEDURE public.pr_criar_produto_alteracao(
	IN a_id_produto integer,
	IN a_tipo tipo_produto_alteracao,
	IN a_anterior character varying,
	IN a_novo character varying)
LANGUAGE 'sql'
AS $BODY$
	INSERT INTO produtos_alteracoes (id_produto, tipo, anterior, novo) VALUES (a_id_produto, a_tipo, a_anterior, a_novo);
$BODY$;

-- FUNCTION: public.fn_produtos_bu0()

-- DROP FUNCTION IF EXISTS public.fn_produtos_bu0();

CREATE OR REPLACE FUNCTION public.fn_produtos_bu0()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
	BEGIN
		NEW.atualizado_em := CURRENT_TIMESTAMP;
		RETURN NEW;
	END;
$BODY$;

-- Trigger: produtos_bu0

-- DROP TRIGGER IF EXISTS produtos_bu0 ON public.produtos;

CREATE OR REPLACE TRIGGER produtos_bu0
    BEFORE UPDATE
    ON public.produtos
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_produtos_bu0();

-- FUNCTION: public.fn_produtos_bu1()

-- DROP FUNCTION IF EXISTS public.fn_produtos_bu1();

CREATE OR REPLACE FUNCTION public.fn_produtos_bu1()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
BEGIN
	IF OLD.descricao <> NEW.descricao THEN
		CALL pr_criar_produto_alteracao(OLD.id, 'descricao', OLD.descricao, NEW.descricao);
	END IF;

	IF OLD.codigo_barra <> NEW.codigo_barra THEN
		CALL pr_criar_produto_alteracao(OLD.id, 'codigo_barra', OLD.codigo_barra, NEW.codigo_barra);
	END IF;

	IF OLD.id_unidade_medida <> NEW.id_unidade_medida THEN
		CALL pr_criar_produto_alteracao(OLD.id, 'id_unidade_medida', OLD.id_unidade_medida, NEW.id_unidade_medida);
	END IF;

	IF OLD.tipo_atividade <> NEW.tipo_atividade THEN
		CALL pr_criar_produto_alteracao(OLD.id, 'tipo_atividade', OLD.tipo_atividade, NEW.tipo_atividade);
	END IF;

	IF OLD.id_ncm <> NEW.id_ncm THEN
		CALL pr_criar_produto_alteracao(OLD.id, 'id_ncm', OLD.id_ncm, NEW.id_ncm);
	END IF;

    IF OLD.codigo_interno <> NEW.codigo_interno THEN
		CALL pr_criar_produto_alteracao(OLD.id, 'codigo_interno', OLD.codigo_interno, NEW.codigo_interno);
	END IF;

    IF OLD.id_anp <> NEW.id_anp THEN
		CALL pr_criar_produto_alteracao(OLD.id, 'id_anp', OLD.id_anp, NEW.id_anp);
	END IF;

	RETURN NEW;
END;
$BODY$;