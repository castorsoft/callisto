-- FUNCTION: public.fn_arquivos_bu0()

-- DROP FUNCTION IF EXISTS public.fn_arquivos_bu0();

CREATE OR REPLACE FUNCTION public.fn_arquivos_bu0()
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

-- Trigger: arquivos_bu0

-- DROP TRIGGER IF EXISTS arquivos_bu0 ON public.arquivos;

CREATE OR REPLACE TRIGGER arquivos_bu0
    BEFORE UPDATE
    ON public.arquivos
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_arquivos_bu0();