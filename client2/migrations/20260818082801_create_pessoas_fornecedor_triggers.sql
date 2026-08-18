-- FUNCTION: public.fn_pessoas_fornecedor_bu0()

-- DROP FUNCTION IF EXISTS public.fn_pessoas_fornecedor_bu0();

CREATE OR REPLACE FUNCTION public.fn_pessoas_fornecedor_bu0()
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

-- Trigger: pessoas_fornecedor_bu0

-- DROP TRIGGER IF EXISTS pessoas_fornecedor_bu0 ON public.pessoas_fornecedor;

CREATE OR REPLACE TRIGGER pessoas_fornecedor_bu0
    BEFORE UPDATE
    ON public.pessoas_fornecedor
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_pessoas_fornecedor_bu0();