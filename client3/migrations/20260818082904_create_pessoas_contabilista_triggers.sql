-- FUNCTION: public.fn_pessoas_contabilista_bu0()

-- DROP FUNCTION IF EXISTS public.fn_pessoas_contabilista_bu0();

CREATE OR REPLACE FUNCTION public.fn_pessoas_contabilista_bu0()
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

-- Trigger: pessoas_contabilista_bu0

-- DROP TRIGGER IF EXISTS pessoas_contabilista_bu0 ON public.pessoas_contabilista;

CREATE OR REPLACE TRIGGER pessoas_contabilista_bu0
    BEFORE UPDATE
    ON public.pessoas_contabilista
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_pessoas_contabilista_bu0();