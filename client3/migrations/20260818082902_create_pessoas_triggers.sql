-- PROCEDURE: public.pr_criar_pessoa_alteracao(integer, tipo_pessoa_alteracao, character varying, character varying)

-- DROP PROCEDURE IF EXISTS public.pr_criar_pessoa_alteracao(integer, tipo_pessoa_alteracao, character varying, character varying);

CREATE OR REPLACE PROCEDURE public.pr_criar_pessoa_alteracao(
	IN a_id_pessoa integer,
	IN a_tipo tipo_pessoa_alteracao,
	IN a_anterior character varying,
	IN a_novo character varying)
LANGUAGE 'sql'
AS $BODY$
	INSERT INTO pessoas_alteracoes (id_pessoa, tipo, anterior, novo) VALUES (a_id_pessoa, a_tipo, a_anterior, a_novo);
$BODY$;

-- FUNCTION: public.fn_pessoas_bu0()

-- DROP FUNCTION IF EXISTS public.fn_pessoas_bu0();

CREATE OR REPLACE FUNCTION public.fn_pessoas_bu0()
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

-- FUNCTION: public.fn_pessoas_bu1()

-- DROP FUNCTION IF EXISTS public.fn_pessoas_bu1();

CREATE OR REPLACE FUNCTION public.fn_pessoas_bu1()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
BEGIN
	IF OLD.cnpj_cpf <> NEW.cnpj_cpf THEN
		CALL pr_criar_pessoa_alteracao(OLD.id, 'cnpj_cpf', OLD.cnpj_cpf, NEW.cnpj_cpf);
	END IF;

	IF OLD.nome <> NEW.nome THEN
		CALL pr_criar_pessoa_alteracao(OLD.id, 'nome', OLD.nome, NEW.nome);
	END IF;

	IF OLD.fantasia <> NEW.fantasia THEN
		CALL pr_criar_pessoa_alteracao(OLD.id, 'fantasia', OLD.fantasia, NEW.fantasia);
	END IF;

	IF OLD.id_endereco <> NEW.id_endereco THEN
		CALL pr_criar_pessoa_alteracao(OLD.id, 'id_endereco', OLD.id_endereco, NEW.id_endereco);
	END IF;

	IF OLD.inscricao_estadual <> NEW.inscricao_estadual THEN
		CALL pr_criar_pessoa_alteracao(OLD.id, 'inscricao_estadual', OLD.inscricao_estadual, NEW.inscricao_estadual);
	END IF;

	RETURN NEW;
END;
$BODY$;

-- Trigger: pessoas_bu0

-- DROP TRIGGER IF EXISTS pessoas_bu0 ON public.pessoas;

CREATE OR REPLACE TRIGGER pessoas_bu0
    BEFORE UPDATE
    ON public.pessoas
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_pessoas_bu0();

-- Trigger: pessoas_bu1

-- DROP TRIGGER IF EXISTS pessoas_bu1 ON public.pessoas;

CREATE OR REPLACE TRIGGER pessoas_bu1
    BEFORE UPDATE
    ON public.pessoas
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_pessoas_bu1();